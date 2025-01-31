#ifndef DEFINE_HPP
#define DEFINE_HPP

#define _PLAY_SOUND_
//#undef _PLAY_SOUND_

#define MAIN_WINDOW_STYLE "\
QMainWindow { \
    background: #2e2f30; \
} \
QDialog { \
    background: #2e2f30; \
} \
QLabel { \
    font-size:   32px; \
    font-weight: bold; \
    color:       white; \
} \
QLabel#dialog { \
    font-size:   14px; \
    font-weight: normal; \
} \
QPushButton { \
    background-color: #52606E; \
    color:            white; \
    font-size:        20px; \
    font-weight:      bold; \
    border-radius:    10px; \
} \
QPushButton:hover { \
    background-color: #708294; \
    color:            white; \
    font-size:        20px; \
    font-weight:      bold; \
} \
QPushButton:disabled { \
    color:            gray; \
} \
QToolTip { \
    border:           1px solid white; \
    background:       #000000; \
    color:            white; \
} \
QPushButton#dialog { \
    background-color: #52606E; \
    color:            white; \
    font-size:        16px; \
    font-weight:      normal; \
    border-radius:    10px; \
    min-width:        80px; \
    max-width:        80px; \
    min-height:       30px; \
    max-height:       30px; \
} \
QPushButton#dialog:hover { \
    background-color: #708294; \
    color:            white; \
    font-size:        16px; \
    font-weight:      normal; \
} \
QPushButton#dialog:disabled { \
    color:            gray; \
}"

// https://fonts.google.com/icons

#endif // DEFINE_HPP
