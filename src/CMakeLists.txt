cmake_minimum_required(VERSION 3.16)

target_include_directories(${CMAKE_PROJECT_NAME}
    PRIVATE 
        .
)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    Utils.cpp
    Graphics.cpp
    Keyboard.cpp
    Timer.cpp
    Texture.cpp
    Palette.cpp
    Font.cpp
    VarWidthFont.cpp
    TextLabel.cpp
    TextInput.cpp
    UIBox.cpp
    TextBox.cpp
    InputBox.cpp
    Hud.cpp
    main.cpp
)