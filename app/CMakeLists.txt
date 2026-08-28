qt_add_executable(predator-rgb
    main.cpp
    controller.cpp
)

set_target_properties(predator-rgb PROPERTIES
    AUTOMOC ON
    WIN32_EXECUTABLE ON
)

qt_policy(SET QTP0001 NEW)

set_source_files_properties(qml/Theme.qml PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_qml_module(predator-rgb
    URI predator_rgb
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/Theme.qml
        qml/Panel.qml
        qml/ZoneSelector.qml
        qml/ColorPicker.qml
        qml/EffectControls.qml
        qml/HueSlider.qml
        qml/SvBox.qml
)

target_link_libraries(predator-rgb PRIVATE core backend Qt6::Gui Qt6::Quick Qt6::QuickControls2)
