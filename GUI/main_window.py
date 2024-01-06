from PySide6.QtWidgets import QMainWindow, QHBoxLayout, QVBoxLayout, QWidget, QGroupBox, QLabel, QPushButton, QLineEdit, QTextEdit
from PySide6.QtGui import QIcon, QPalette, QColor, QFont
from PySide6.QtCore import Qt, QTimer
import pyqtgraph as pg
from mcn import getValueFromSensor, sendValueToMCN

class MainWindow(QMainWindow):
    promotie: str = "2023-2024"
    team: list[str] = [
        "Miron-Onciul Tudor",
        "Jipianu Mihnea",
    ]
    def __init__(self):
        super().__init__()
        self.setWindowTitle(f"Proiect Microprocesoare {self.promotie}")
        self.setWindowIcon(QIcon("./icon.png"))
        

        primary_layout = QVBoxLayout()
        secondary_layout = QHBoxLayout()
        tertiary_layout = QVBoxLayout()

        team_box = QGroupBox("Membrii echipei")
        bold_font = QFont()
        bold_font.setBold(True)
        team_box.setFont(bold_font)

        first_member = QLabel(f"Membru 1: {self.team[0]}")
        second_member = QLabel(f"Membru 2: {self.team[1]}")
        team_box_layout = QVBoxLayout()
        team_box_layout.addWidget(first_member,1)
        team_box_layout.addWidget(second_member,1)
        team_box.setLayout(team_box_layout)

        control_panel_box = QGroupBox("Control Panel")
        control_panel_box.setFont(bold_font)

        button3 = QPushButton("Change Led Direction")
        button3.clicked.connect(self.send_input)
 
        control_panel_box_layout = QVBoxLayout()
        control_panel_box_layout.addWidget(button3,1)
        control_panel_box.setLayout(control_panel_box_layout)

        tertiary_layout.addWidget(team_box, 1)
        tertiary_layout.addWidget(control_panel_box,5)
                
        self.plot_widget = pg.PlotWidget()
        self.second = [1,2,3,4,5]
        self.flameValues = []
        for i in range(5):
            self.flameValues.append(getValueFromSensor())
                
        self.color_ranges = [(0, 2.0, 'g'), (2.0, 4.0, 'y'), (4.0, 6.0, 'r')]

        brush_colors = []
        for value in self.flameValues:
            for min_val, max_val, color in self.color_ranges:
                if min_val <= value <= max_val:
                    brush_colors.append(color)
                    break
        
        
        bars = pg.BarGraphItem(x=self.second, height=self.flameValues, width=0.5, brushes=brush_colors)
        self.plot_widget.addItem(bars)
                                
        secondary_layout.addWidget(self.plot_widget, 3)                                 
        
        secondary_layout.addLayout(tertiary_layout, 1)
        primary_layout.addLayout(secondary_layout, 4)

        widget = QWidget()
        widget.setLayout(primary_layout)
        
        self.setCentralWidget(widget)
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.uptdatePlot)
        self.timer.start(1000) 
        
    def send_input(self):
        sendValueToMCN('c')
        
    def uptdatePlot(self):
        self.flameValues = self.flameValues[1:]+[getValueFromSensor()]
        print(self.flameValues)
        
        brush_colors = []
        for value in self.flameValues:
            for min_val, max_val, color in self.color_ranges:
                if min_val <= value <= max_val:
                    brush_colors.append(color)
                    break
        
        bars = pg.BarGraphItem(x=self.second, height=self.flameValues, width=0.5, brushes=brush_colors)
        self.plot_widget.clear()
        self.plot_widget.addItem(bars)