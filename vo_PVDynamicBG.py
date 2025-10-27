# -*- coding: utf-8 -*-
import sys
import os
import cv2
import numpy as np
from PyQt5.QtWidgets import (
    QApplication, QMainWindow, QWidget, QLabel, QPushButton, QFileDialog,
    QVBoxLayout, QHBoxLayout, QListWidget, QSlider, QMessageBox,
    QSizePolicy, QStyleFactory, QProgressDialog
)
from PyQt5.QtCore import Qt, QTimer
from PyQt5.QtGui import QImage, QPixmap
from PyQt5.QtWidgets import QCheckBox

class VideoData:
    def __init__(self, filepath, frames, fps, width, height, name=None):
        self.filepath = filepath
        self.frames = frames
        self.fps = fps
        self.width = width
        self.height = height
        self.name = name or os.path.basename(filepath)
        self.use_moving_bg = False  

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("跑步姿勢影片疊圖工具")
        self.resize(960, 720)
        self.setStyle(QStyleFactory.create("Fusion"))
        self.videos = []
        self.current_video = None
        self.composite_frames = []
        self.play_timer = QTimer()
        self.play_timer.timeout.connect(self.play_frame)
        self.current_frame_idx = 0
        self.clip_start = 0
        self.clip_end = 0
        self.is_editing = False
        self.fg_weight = 100
        self.bg_weight = 100
        self.blur_size = 1
        self.init_ui()

    def init_ui(self):
        self.video_label = QLabel("影片區")
        self.video_label.setAlignment(Qt.AlignCenter)
        self.video_label.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
        self.video_label.setScaledContents(False)

        self.frame_label = QLabel("當前幀：0")
        self.frame_label.setAlignment(Qt.AlignCenter)

        self.slider_main = QSlider(Qt.Horizontal)
        self.slider_main.setEnabled(False)
        self.slider_main.valueChanged.connect(self.slider_changed)

        self.set_start_btn = QPushButton("設為起點")
        self.set_start_btn.setEnabled(False)
        self.set_start_btn.clicked.connect(self.set_clip_start)

        self.set_end_btn = QPushButton("設為終點")
        self.set_end_btn.setEnabled(False)
        self.set_end_btn.clicked.connect(self.set_clip_end)

        self.clip_range_label = QLabel("裁剪範圍：尚未設定")
        self.clip_range_label.setAlignment(Qt.AlignCenter)

        self.slider_fg = QSlider(Qt.Vertical)
        self.slider_fg.setMinimum(100)
        self.slider_fg.setMaximum(400)
        self.slider_fg.setValue(int(self.fg_weight * 100))
        self.slider_fg.sliderReleased.connect(self.update_fg_weight_released)

        self.slider_bg = QSlider(Qt.Vertical)
        self.slider_bg.setMinimum(0)
        self.slider_bg.setMaximum(100)
        self.slider_bg.setValue(int(self.bg_weight * 100))
        self.slider_bg.sliderReleased.connect(self.update_bg_weight_released)

        self.slider_blur = QSlider(Qt.Vertical)
        self.slider_blur.setMinimum(1)
        self.slider_blur.setMaximum(50)
        self.slider_blur.setValue(self.blur_size)
        self.slider_blur.sliderReleased.connect(self.update_blur_size_released)

        self.edge_checkbox = QCheckBox("邊緣加強")
        self.edge_checkbox.setChecked(False)
        self.edge_slider = QSlider(Qt.Horizontal)
        self.edge_slider.setMinimum(0)
        self.edge_slider.setMaximum(100)
        self.edge_slider.setValue(50)

        self.center_checkbox = QCheckBox("中心加權")
        self.center_checkbox.setChecked(False)
        self.center_slider = QSlider(Qt.Horizontal)
        self.center_slider.setMinimum(0)
        self.center_slider.setMaximum(100)
        self.center_slider.setValue(80)

        self.moving_bg_checkbox = QCheckBox("使用動態背景")
        self.moving_bg_checkbox.setChecked(False)

        self.video_list = QListWidget()
        self.video_list.setSelectionMode(QListWidget.MultiSelection)
        self.video_list.setMaximumHeight(60)

        self.play_btn = QPushButton("播放")
        self.play_btn.clicked.connect(self.toggle_play_pause)

        load_btn = QPushButton("讀取影片")
        load_btn.clicked.connect(self.load_videos)

        remove_btn = QPushButton("移除影片")
        remove_btn.clicked.connect(self.remove_video)

        edit_btn = QPushButton("進入裁剪模式")
        edit_btn.clicked.connect(self.edit_clip)

        save_clip_btn = QPushButton("確認裁剪")
        save_clip_btn.clicked.connect(self.save_clip)

        compose_btn = QPushButton("一般合成")
        compose_btn.clicked.connect(self.compose_videos)

        std_compose_btn = QPushButton("標準化合成")
        std_compose_btn.clicked.connect(self.compose_videos_standardized)

        side_by_side_btn = QPushButton("並排合成")
        side_by_side_btn.clicked.connect(self.compose_videos_side_by_side)

        std_side_by_side_btn = QPushButton("標準化並排合成")
        std_side_by_side_btn.clicked.connect(self.compose_videos_side_by_side_standardized)

        export_btn = QPushButton("儲存影片")
        export_btn.clicked.connect(self.export_composite)

        preview_btn = QPushButton("預覽合成第一幀")
        preview_btn.clicked.connect(self.preview_composite_frame)

        # 左側佈局
        left_layout = QVBoxLayout()
        btn_layout = QHBoxLayout()
        for btn in [load_btn, remove_btn, self.play_btn, edit_btn, self.set_start_btn, self.set_end_btn,
                    save_clip_btn, compose_btn, std_compose_btn, side_by_side_btn, std_side_by_side_btn, export_btn]:
            btn_layout.addWidget(btn)
        left_layout.addLayout(btn_layout)
        left_layout.addWidget(self.video_label)
        left_layout.addWidget(self.frame_label)
        left_layout.addWidget(QLabel("影片控制"))
        left_layout.addWidget(self.slider_main)
        left_layout.addWidget(self.clip_range_label)
        left_layout.addWidget(preview_btn)
        left_layout.addWidget(self.video_list)

        # 右側佈局（含滑桿與強化功能）
        right_layout = QVBoxLayout()
        right_layout.setAlignment(Qt.AlignTop)

        for label_text, slider in [("前景強度", self.slider_fg), ("背景強度", self.slider_bg), ("遮罩模糊", self.slider_blur)]:
            block = QVBoxLayout()
            block.setAlignment(Qt.AlignHCenter)
            label = QLabel(label_text)
            label.setAlignment(Qt.AlignCenter)
            block.addWidget(label)
            block.addWidget(slider)
            right_layout.addLayout(block)
            right_layout.addSpacing(10)

        edge_block = QVBoxLayout()
        edge_block.setAlignment(Qt.AlignHCenter)
        edge_block.addWidget(self.edge_checkbox)
        edge_block.addWidget(self.edge_slider)
        right_layout.addLayout(edge_block)

        center_block = QVBoxLayout()
        center_block.setAlignment(Qt.AlignHCenter)
        center_block.addWidget(self.center_checkbox)
        center_block.addWidget(self.center_slider)
        right_layout.addLayout(center_block)

        right_layout.addWidget(self.moving_bg_checkbox)

        main_layout = QHBoxLayout()
        main_layout.addLayout(left_layout, stretch=12)
        main_layout.addLayout(right_layout, stretch=1)

        container = QWidget()
        container.setLayout(main_layout)
        self.setCentralWidget(container)

    def compose_videos_side_by_side_standardized(self):
        selected = self.video_list.selectedItems()
        if len(selected) < 2:
            QMessageBox.warning(self, "錯誤", "請選擇兩部以上影片")
            return

        # ➤ 詢問用戶合併方式：水平或垂直
        msg = QMessageBox(self)
        msg.setWindowTitle("合併方式選擇")
        msg.setText("請選擇影片並排方式")
        horizontal_btn = msg.addButton("水平拼接", QMessageBox.AcceptRole)
        vertical_btn = msg.addButton("垂直拼接", QMessageBox.RejectRole)
        msg.exec_()

        concat_mode = "horizontal" if msg.clickedButton() == horizontal_btn else "vertical"

        # 取得選取的影片
        selected_videos = [self.videos[self.video_list.row(item)] for item in selected]
        max_len = max(len(v.frames) for v in selected_videos)

        # 先對所有影片做重採樣至相同幀數
        resampled_videos = []
        for v in selected_videos:
            new_frames = self.resample_frames(v.frames, max_len)
            resampled_videos.append(VideoData(
                v.filepath, new_frames, v.fps, v.width, v.height, name=v.name + "_resampled"
            ))

        # 並排合成
        self.composite_frames = []
        progress = QProgressDialog("正在標準化並排合成影片...", "取消", 0, max_len, self)
        progress.setWindowModality(Qt.WindowModal)
        progress.setMinimumDuration(0)

        for i in range(max_len):
            row_frames = [v.frames[i] for v in resampled_videos]

            # 根據合併方式統一尺寸（必要時調整）
            if concat_mode == "horizontal":
                # 統一高度
                target_height = row_frames[0].shape[0]
                for idx, f in enumerate(row_frames):
                    if f.shape[0] != target_height:
                        scale = target_height / f.shape[0]
                        new_width = int(f.shape[1] * scale)
                        row_frames[idx] = cv2.resize(f, (new_width, target_height), interpolation=cv2.INTER_AREA)
                composite = cv2.hconcat(row_frames)
            else:
                # 垂直拼接 → 統一寬度
                target_width = row_frames[0].shape[1]
                for idx, f in enumerate(row_frames):
                    if f.shape[1] != target_width:
                        scale = target_width / f.shape[1]
                        new_height = int(f.shape[0] * scale)
                        row_frames[idx] = cv2.resize(f, (target_width, new_height), interpolation=cv2.INTER_AREA)
                composite = cv2.vconcat(row_frames)

            self.composite_frames.append(composite)
            progress.setValue(i + 1)
            if progress.wasCanceled():
                break
        progress.close()

        # 統一寬高
        if concat_mode == "horizontal":
            total_width = sum(f.shape[1] for f in row_frames)
            total_height = row_frames[0].shape[0]
        else:
            total_width = row_frames[0].shape[1]
            total_height = sum(f.shape[0] for f in row_frames)

        name = "標準化並排影片（水平）" if concat_mode == "horizontal" else "標準化並排影片（垂直）"
        self.current_video = VideoData(name, self.composite_frames,
                                    resampled_videos[0].fps,
                                    total_width, total_height,
                                    name)
        self.videos.append(self.current_video)
        self.video_list.addItem(self.current_video.name)
        self.current_frame_idx = 0
        self.slider_main.setMaximum(len(self.composite_frames) - 1)
        self.slider_main.setValue(0)
        self.slider_main.setEnabled(True)
        self.play_timer.start(int(1000 / self.current_video.fps))
        self.play_btn.setText("暫停")

    def compose_videos_side_by_side(self):
        selected = self.video_list.selectedItems()
        if len(selected) < 2:
            QMessageBox.warning(self, "錯誤", "請選擇兩部以上影片")
            return

        # ➤ 詢問使用者合併方式
        msg = QMessageBox(self)
        msg.setWindowTitle("合併方式選擇")
        msg.setText("請選擇影片並排方式")
        horizontal_btn = msg.addButton("水平拼接", QMessageBox.AcceptRole)
        vertical_btn = msg.addButton("垂直拼接", QMessageBox.RejectRole)
        msg.exec_()

        concat_mode = "horizontal" if msg.clickedButton() == horizontal_btn else "vertical"

        # 取得影片與最大幀長
        selected_videos = [self.videos[self.video_list.row(item)] for item in selected]
        max_len = max(len(v.frames) for v in selected_videos)

        self.composite_frames = []
        progress = QProgressDialog("正在並排合成影片...", "取消", 0, max_len, self)
        progress.setWindowModality(Qt.WindowModal)
        progress.setMinimumDuration(0)

        for i in range(max_len):
            row_frames = []
            for v in selected_videos:
                if i < len(v.frames):
                    frame = v.frames[i]
                else:
                    frame = v.frames[-1]  # 補最後一幀
                row_frames.append(frame)

            # 統一尺寸
            if concat_mode == "horizontal":
                target_height = row_frames[0].shape[0]
                for idx, f in enumerate(row_frames):
                    if f.shape[0] != target_height:
                        scale = target_height / f.shape[0]
                        new_width = int(f.shape[1] * scale)
                        row_frames[idx] = cv2.resize(f, (new_width, target_height), interpolation=cv2.INTER_AREA)
                composite = cv2.hconcat(row_frames)
            else:
                target_width = row_frames[0].shape[1]
                for idx, f in enumerate(row_frames):
                    if f.shape[1] != target_width:
                        scale = target_width / f.shape[1]
                        new_height = int(f.shape[0] * scale)
                        row_frames[idx] = cv2.resize(f, (target_width, new_height), interpolation=cv2.INTER_AREA)
                composite = cv2.vconcat(row_frames)

            self.composite_frames.append(composite)
            progress.setValue(i + 1)
            if progress.wasCanceled():
                break
        progress.close()

        # 計算合成影片尺寸
        if concat_mode == "horizontal":
            total_width = sum(f.shape[1] for f in row_frames)
            total_height = row_frames[0].shape[0]
        else:
            total_width = row_frames[0].shape[1]
            total_height = sum(f.shape[0] for f in row_frames)

        name = "並排影片（水平）" if concat_mode == "horizontal" else "並排影片（垂直）"
        self.current_video = VideoData(name, self.composite_frames,
                                    selected_videos[0].fps,
                                    total_width, total_height,
                                    name)
        self.videos.append(self.current_video)
        self.video_list.addItem(self.current_video.name)
        self.current_frame_idx = 0
        self.slider_main.setMaximum(len(self.composite_frames) - 1)
        self.slider_main.setValue(0)
        self.slider_main.setEnabled(True)
        self.play_timer.start(int(1000 / self.current_video.fps))
        self.play_btn.setText("暫停")

    def compute_background(self, frames, idx, window=15):
        half = window // 2
        start = max(0, idx - half)
        end = min(len(frames), idx + half + 1)
        if end - start == 0:
            return frames[idx]  # fallback
        stack = np.stack(frames[start:end], axis=0)
        return np.median(stack, axis=0)

    def resample_frames(self, frames, target_len):
        original_len = len(frames)
        if original_len == target_len:
            return frames
        indices = np.linspace(0, original_len - 1, target_len).astype(np.int32)
        return [frames[i] for i in indices]

    def update_fg_weight_released(self):
        self.fg_weight = self.slider_fg.value() / 100.0
        self.preview_composite_frame()

    def update_bg_weight_released(self):
        self.bg_weight = self.slider_bg.value() / 100.0
        self.preview_composite_frame()

    def update_blur_size_released(self):
        val = self.slider_blur.value()
        if val % 2 == 0:
            val += 1  # 模糊核需為奇數
        self.blur_size = val
        self.preview_composite_frame()

    def enhance_mask(self, mask, gray):
        # 輸入：原始二值遮罩（0~1）、原始灰階圖像（0~255）
        # 邊緣加強
        if self.edge_checkbox.isChecked():
            edge_strength = self.edge_slider.value() / 100.0
            edges = cv2.Canny(gray, 50, 150)
            edges = edges.astype(np.float32) / 255.0
            mask += edges * edge_strength
            mask = np.clip(mask, 0, 1)

        # 中心加權
        if self.center_checkbox.isChecked():
            center_strength = self.center_slider.value() / 100.0
            dist = cv2.distanceTransform((mask * 255).astype(np.uint8), cv2.DIST_L2, 3)
            dist = cv2.normalize(dist, None, 0, 1.0, cv2.NORM_MINMAX)
            mask *= (1.0 - center_strength) + dist * center_strength
            mask = np.clip(mask, 0, 1)

        return mask

    def play_frame(self):
        if self.current_video and self.current_frame_idx < len(self.current_video.frames):
            frame = self.current_video.frames[self.current_frame_idx]
            self.show_frame(frame)
            self.slider_main.blockSignals(True)
            self.slider_main.setValue(self.current_frame_idx)
            self.slider_main.blockSignals(False)
            self.current_frame_idx += 1
        else:
            self.play_timer.stop()
            self.play_btn.setText("播放")

    def show_frame(self, frame):
        rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        h, w, ch = rgb.shape
        bytes_per_line = ch * w
        qimg = QImage(rgb.data, w, h, bytes_per_line, QImage.Format_RGB888)
        pixmap = QPixmap.fromImage(qimg)
        self.video_label.setPixmap(pixmap.scaled(self.video_label.size(), Qt.KeepAspectRatio, Qt.SmoothTransformation))
        self.frame_label.setText(f"當前幀：{self.current_frame_idx}")

    def preview_composite_frame(self):
        selected = self.video_list.selectedItems()
        if len(selected) < 2:
            QMessageBox.warning(self, "錯誤", "請選擇兩部以上影片")
            return

        selected_videos = [self.videos[self.video_list.row(item)] for item in selected]
        idx = 0  # 預覽第 0 幀

        backgrounds = []
        for v in selected_videos:
            stack = np.stack(v.frames[:min(30, len(v.frames))], axis=0)
            background = np.median(stack, axis=0).astype(np.uint8)
            backgrounds.append(background)

        blended = np.zeros_like(selected_videos[0].frames[0], dtype=np.float32)
        for vid_idx, v in enumerate(selected_videos):
            frame = v.frames[idx].astype(np.float32)
            background = backgrounds[vid_idx].astype(np.float32)

            fg_mask = cv2.absdiff(frame, background)
            gray = cv2.cvtColor(fg_mask.astype(np.uint8), cv2.COLOR_BGR2GRAY)
            _, mask = cv2.threshold(gray, 30, 1, cv2.THRESH_BINARY)
            mask = cv2.GaussianBlur(mask.astype(np.float32), (self.blur_size, self.blur_size), 0)
            mask = self.enhance_mask(mask, gray)
            mask = self.enhance_mask(mask, gray)
            mask_3c = cv2.merge([mask] * 3)

            fg = frame * mask_3c * self.fg_weight
            bg = background * (1 - mask_3c) * self.bg_weight
            composite = fg + bg
            blended += composite

        blended = np.clip(blended / len(selected_videos), 0, 255).astype(np.uint8)
        self.show_frame(blended)

    def set_clip_start(self):
        self.clip_start = self.slider_main.value()
        self.clip_range_label.setText(f"裁剪範圍：起點 {self.clip_start} ~ 終點 {self.clip_end}")

    def set_clip_end(self):
        self.clip_end = self.slider_main.value()
        self.clip_range_label.setText(f"裁剪範圍：起點 {self.clip_start} ~ 終點 {self.clip_end}")

    def load_videos(self):
        paths, _ = QFileDialog.getOpenFileNames(self, "選擇影片", "", "影片 (*.mp4 *.avi)")
        self.videos.clear()
        self.video_list.clear()
        for path in paths:
            cap = cv2.VideoCapture(path)
            fps = cap.get(cv2.CAP_PROP_FPS)
            width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
            height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
            total_frames = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))

            progress = QProgressDialog(f"正在讀取：{os.path.basename(path)}", "取消", 0, total_frames, self)
            progress.setWindowModality(Qt.WindowModal)
            progress.setMinimumDuration(0)

            frames = []
            for i in range(total_frames):
                ret, frame = cap.read()
                if not ret:
                    break
                frames.append(frame)
                progress.setValue(i + 1)
                if progress.wasCanceled():
                    break
            cap.release()
            progress.close()
            name = os.path.basename(path)
            self.videos.append(VideoData(path, frames, fps, width, height, name))
            self.video_list.addItem(name)

    def compose_videos(self):
        selected = self.video_list.selectedItems()
        if len(selected) < 2:
            QMessageBox.warning(self, "錯誤", "請選擇兩部以上影片合成")
            return

        selected_videos = [self.videos[self.video_list.row(item)] for item in selected]
        max_len = max(len(v.frames) for v in selected_videos)

        backgrounds = []
        for v in selected_videos:
            stack = np.stack(v.frames[:min(30, len(v.frames))], axis=0)
            background = np.median(stack, axis=0).astype(np.uint8)
            backgrounds.append(background)

        self.composite_frames = []
        progress = QProgressDialog("正在合成影片...", "取消", 0, max_len, self)
        progress.setWindowModality(Qt.WindowModal)
        progress.setMinimumDuration(0)

        for i in range(max_len):
            blended = np.zeros_like(selected_videos[0].frames[0], dtype=np.float32)
            for idx, v in enumerate(selected_videos):
                if i < len(v.frames):
                    frame = v.frames[i].astype(np.float32)
                else:
                    frame = v.frames[-1].astype(np.float32)

                if self.moving_bg_checkbox.isChecked():
                    background = self.compute_background(v.frames, i, window=15).astype(np.float32)
                else:
                    background = backgrounds[idx].astype(np.float32)

                fg_mask = cv2.absdiff(frame, background)
                gray = cv2.cvtColor(fg_mask.astype(np.uint8), cv2.COLOR_BGR2GRAY)
                _, mask = cv2.threshold(gray, 30, 1, cv2.THRESH_BINARY)
                mask = cv2.GaussianBlur(mask.astype(np.float32), (self.blur_size, self.blur_size), 0)
                mask = self.enhance_mask(mask, gray)
                mask_3c = cv2.merge([mask] * 3)

                fg = frame * mask_3c * self.fg_weight
                bg = background * (1 - mask_3c) * self.bg_weight
                composite = fg + bg
                blended += composite

            blended = np.clip(blended / len(selected_videos), 0, 255).astype(np.uint8)
            self.composite_frames.append(blended)
            progress.setValue(i + 1)
            if progress.wasCanceled():
                break
        progress.close()

        self.current_video = VideoData("合成影片", self.composite_frames,
                                    selected_videos[0].fps,
                                    selected_videos[0].width,
                                    selected_videos[0].height,
                                    "合成影片")
        self.videos.append(self.current_video)
        self.video_list.addItem(self.current_video.name)
        self.current_frame_idx = 0
        self.slider_main.setMaximum(len(self.composite_frames) - 1)
        self.slider_main.setValue(0)
        self.slider_main.setEnabled(True)
        self.play_timer.start(int(1000 / self.current_video.fps))
        self.play_btn.setText("暫停")

    def compose_videos_standardized(self):
        selected = self.video_list.selectedItems()
        if len(selected) < 2:
            QMessageBox.warning(self, "錯誤", "請選擇兩部以上影片合成")
            return

        selected_videos = [self.videos[self.video_list.row(item)] for item in selected]
        max_len = max(len(v.frames) for v in selected_videos)

        # 標準化重採樣
        resampled_videos = []
        for v in selected_videos:
            new_frames = self.resample_frames(v.frames, max_len)
            resampled_videos.append(VideoData(
                v.filepath, new_frames, v.fps, v.width, v.height, name=v.name + "_resampled"
            ))

        # 建立背景
        backgrounds = []
        for v in resampled_videos:
            stack = np.stack(v.frames[:min(30, len(v.frames))], axis=0)
            background = np.median(stack, axis=0).astype(np.uint8)
            backgrounds.append(background)

        self.composite_frames = []
        progress = QProgressDialog("正在標準化合成影片...", "取消", 0, max_len, self)
        progress.setWindowModality(Qt.WindowModal)
        progress.setMinimumDuration(0)

        for i in range(max_len):
            blended = np.zeros_like(resampled_videos[0].frames[0], dtype=np.float32)
            for idx, v in enumerate(resampled_videos):
                frame = v.frames[i].astype(np.float32)

                if self.moving_bg_checkbox.isChecked():
                    background = self.compute_background(v.frames, i, window=15).astype(np.float32)
                else:
                    background = backgrounds[idx].astype(np.float32)

                fg_mask = cv2.absdiff(frame, background)
                gray = cv2.cvtColor(fg_mask.astype(np.uint8), cv2.COLOR_BGR2GRAY)
                _, mask = cv2.threshold(gray, 30, 1, cv2.THRESH_BINARY)
                mask = cv2.GaussianBlur(mask.astype(np.float32), (self.blur_size, self.blur_size), 0)
                mask = self.enhance_mask(mask, gray)
                mask_3c = cv2.merge([mask] * 3)

                fg = frame * mask_3c * self.fg_weight
                bg = background * (1 - mask_3c) * self.bg_weight
                composite = fg + bg
                blended += composite

            blended = np.clip(blended / len(resampled_videos), 0, 255).astype(np.uint8)
            self.composite_frames.append(blended)
            progress.setValue(i + 1)
            if progress.wasCanceled():
                break
        progress.close()

        self.current_video = VideoData("標準化合成影片", self.composite_frames,
                                    selected_videos[0].fps,
                                    selected_videos[0].width,
                                    selected_videos[0].height,
                                    "標準化合成影片")
        self.videos.append(self.current_video)
        self.video_list.addItem(self.current_video.name)
        self.current_frame_idx = 0
        self.slider_main.setMaximum(len(self.composite_frames) - 1)
        self.slider_main.setValue(0)
        self.slider_main.setEnabled(True)
        self.play_timer.start(int(1000 / self.current_video.fps))
        self.play_btn.setText("暫停")

    def export_composite(self):
        selected = self.video_list.selectedItems()
        if len(selected) != 1:
            QMessageBox.warning(self, "錯誤", "請選擇一部影片儲存")
            return

        idx = self.video_list.row(selected[0])
        video = self.videos[idx]

        save_path, _ = QFileDialog.getSaveFileName(self, "儲存影片", f"{video.name}", "影片 (*.mp4)")
        if not save_path:
            return

        h, w = video.height, video.width
        fourcc = cv2.VideoWriter_fourcc(*'mp4v')
        out = cv2.VideoWriter(save_path, fourcc, video.fps, (w, h))

        progress = QProgressDialog("正在儲存影片...", "取消", 0, len(video.frames), self)
        progress.setWindowModality(Qt.WindowModal)
        progress.setMinimumDuration(0)

        for i, f in enumerate(video.frames):
            out.write(f)
            progress.setValue(i + 1)
            if progress.wasCanceled():
                break
        out.release()
        progress.close()
        QMessageBox.information(self, "完成", f"{video.name} 已成功儲存")

    def slider_changed(self, value):
        if self.current_video:
            self.current_frame_idx = value
            frame = self.current_video.frames[value]
            self.show_frame(frame)

    def toggle_play_pause(self):
        if self.play_timer.isActive():
            self.play_timer.stop()
            self.play_btn.setText("播放")
        else:
            selected = self.video_list.selectedItems()
            if len(selected) != 1:
                QMessageBox.warning(self, "錯誤", "請選擇一部影片播放")
                return

            idx = self.video_list.row(selected[0])
            self.current_video = self.videos[idx]
            self.current_frame_idx = 0
            self.slider_main.setMaximum(len(self.current_video.frames) - 1)
            self.slider_main.setEnabled(True)

            self.play_timer.start(int(1000 / self.current_video.fps))
            self.play_btn.setText("暫停")

    def save_clip(self):
        if not self.current_video:
            QMessageBox.warning(self, "錯誤", "請先進入編輯模式")
            return
        if self.clip_end <= self.clip_start:
            QMessageBox.warning(self, "錯誤", "結束點需大於開始點")
            return
        clip_frames = self.current_video.frames[self.clip_start:self.clip_end + 1]
        new_name = f"clip_{self.current_video.name}"
        new_video = VideoData("剪輯自記憶體", clip_frames, self.current_video.fps, self.current_video.width, self.current_video.height, new_name)
        self.videos.append(new_video)
        self.video_list.addItem(new_name)

    def edit_clip(self):
        selected = self.video_list.selectedItems()
        if len(selected) != 1:
            QMessageBox.warning(self, "錯誤", "請選擇一部影片剪輯")
            return
        idx = self.video_list.row(selected[0])
        self.current_video = self.videos[idx]
        max_frame = len(self.current_video.frames) - 1
        self.clip_start = 0
        self.clip_end = max_frame

        self.slider_main.setMaximum(max_frame)
        self.slider_main.setEnabled(True)
        self.set_start_btn.setEnabled(True)
        self.set_end_btn.setEnabled(True)

        self.clip_range_label.setText("裁剪範圍：尚未設定")
        QMessageBox.information(self, "剪輯模式", "請使用滑桿選擇幀，點按鈕設定起點與終點，然後按『確認裁剪』")

    def remove_video(self):
        selected = self.video_list.selectedItems()
        for item in selected:
            row = self.video_list.row(item)
            self.video_list.takeItem(row)
            del self.videos[row]

    def update_clip_start(self, value):
        self.clip_start = value
        self.slider_main.setValue(value)
        self.slider_end.setMinimum(value + 1)
        if self.current_video:
            self.current_frame_idx = value
            self.show_frame(self.current_video.frames[value])

    def update_clip_end(self, value):
        self.clip_end = value
        if self.current_video:
            self.current_frame_idx = value
            self.show_frame(self.current_video.frames[value])

if __name__ == '__main__':
    app = QApplication(sys.argv)
    w = MainWindow()
    w.show()
    sys.exit(app.exec_())