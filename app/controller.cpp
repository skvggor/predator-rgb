#include "controller.h"

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>

#include "backend/device_backend.h"
#include "core/color.h"
#include "core/effect.h"

namespace predator {

namespace {

Mode mode_from_display(const QString& text) {
    if (text == "Static") return Mode::Static;
    if (text == "Breath") return Mode::Breath;
    if (text == "Neon") return Mode::Neon;
    if (text == "Wave") return Mode::Wave;
    if (text == "Shifting") return Mode::Shifting;
    if (text == "Zoom") return Mode::Zoom;
    return Mode::Wave;
}

Direction direction_from_display(const QString& text) {
    return (text == "Right to Left") ? Direction::RightToLeft : Direction::LeftToRight;
}

QColor to_qcolor(Color color) {
    return QColor(color.red, color.green, color.blue);
}

Color to_color(const QColor& color) {
    return {static_cast<uint8_t>(color.red()), static_cast<uint8_t>(color.green()), static_cast<uint8_t>(color.blue())};
}

} // namespace

Controller::Controller(QObject* parent) : QObject(parent) {
    loadConfig();
    auto initial = currentColor();
    if (!zone1Color_.isValid()) zone1Color_ = initial;
    if (!zone2Color_.isValid()) zone2Color_ = initial;
    if (!zone3Color_.isValid()) zone3Color_ = initial;
    if (!zone4Color_.isValid()) zone4Color_ = initial;
}

QString Controller::configPath() const {
    auto configDir = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    return configDir + QStringLiteral("/predator-rgb/config.json");
}

void Controller::saveConfig() {
    QJsonObject obj;
    obj[QStringLiteral("hue")] = hue_;
    obj[QStringLiteral("saturation")] = static_cast<double>(saturation_);
    obj[QStringLiteral("value")] = static_cast<double>(value_);
    obj[QStringLiteral("mode")] = mode_;
    obj[QStringLiteral("speed")] = speed_;
    obj[QStringLiteral("brightness")] = brightness_;
    obj[QStringLiteral("direction")] = direction_;
    obj[QStringLiteral("zonesLinked")] = zonesLinked_;
    obj[QStringLiteral("zone1Color")] = zone1Color_.name();
    obj[QStringLiteral("zone2Color")] = zone2Color_.name();
    obj[QStringLiteral("zone3Color")] = zone3Color_.name();
    obj[QStringLiteral("zone4Color")] = zone4Color_.name();

    QFile file(configPath());
    auto dir = QFileInfo(file).absoluteDir();
    if (!dir.exists()) {
        dir.mkpath(QStringLiteral("."));
    }
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(obj).toJson());
    }
}

void Controller::loadConfig() {
    QFile file(configPath());
    if (!file.open(QIODevice::ReadOnly)) return;

    auto doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isObject()) return;

    auto obj = doc.object();
    hue_ = obj.value(QStringLiteral("hue")).toInt(340);
    saturation_ = obj.value(QStringLiteral("saturation")).toDouble(0.76);
    value_ = obj.value(QStringLiteral("value")).toDouble(1.0);
    mode_ = obj.value(QStringLiteral("mode")).toString(QStringLiteral("Wave"));
    speed_ = obj.value(QStringLiteral("speed")).toInt(5);
    brightness_ = obj.value(QStringLiteral("brightness")).toInt(80);
    direction_ = obj.value(QStringLiteral("direction")).toString(QStringLiteral("Left to Right"));
    zonesLinked_ = obj.value(QStringLiteral("zonesLinked")).toBool(true);
    zone1Color_ = QColor(obj.value(QStringLiteral("zone1Color")).toString());
    zone2Color_ = QColor(obj.value(QStringLiteral("zone2Color")).toString());
    zone3Color_ = QColor(obj.value(QStringLiteral("zone3Color")).toString());
    zone4Color_ = QColor(obj.value(QStringLiteral("zone4Color")).toString());

    emit hueChanged();
    emit saturationChanged();
    emit valueChanged();
    emit currentColorChanged();
    emit modeChanged();
    emit speedChanged();
    emit brightnessChanged();
    emit directionChanged();
    emit zonesLinkedChanged();
    emit zone1ColorChanged();
    emit zone2ColorChanged();
    emit zone3ColorChanged();
    emit zone4ColorChanged();

    setHexText(to_color(currentColor()).to_hex().c_str());
}

Color Controller::currentRgb() const {
    return hsv_to_rgb({static_cast<float>(hue_), saturation_, value_});
}

QColor Controller::currentColor() const {
    return to_qcolor(currentRgb());
}

void Controller::setHue(int value) {
    if (hue_ == value) return;
    hue_ = value;
    emit hueChanged();
    emit currentColorChanged();
    saveConfig();
}

void Controller::setSaturation(float value) {
    if (saturation_ == value) return;
    saturation_ = value;
    emit saturationChanged();
    emit currentColorChanged();
    saveConfig();
}

void Controller::setValue(float value) {
    if (value_ == value) return;
    value_ = value;
    emit valueChanged();
    emit currentColorChanged();
    saveConfig();
}

void Controller::setMode(const QString& value) {
    if (mode_ == value) return;
    mode_ = value;
    emit modeChanged();
    saveConfig();
}

void Controller::setSpeed(int value) {
    if (speed_ == value) return;
    speed_ = value;
    emit speedChanged();
    saveConfig();
}

void Controller::setBrightness(int value) {
    if (brightness_ == value) return;
    brightness_ = value;
    emit brightnessChanged();
    saveConfig();
}

void Controller::setDirection(const QString& value) {
    if (direction_ == value) return;
    direction_ = value;
    emit directionChanged();
    saveConfig();
}

void Controller::setZonesLinked(bool value) {
    if (zonesLinked_ == value) return;
    zonesLinked_ = value;
    emit zonesLinkedChanged();
    saveConfig();
}

void Controller::setSelectedZone(int value) {
    if (selectedZone_ == value) return;
    selectedZone_ = value;
    emit selectedZoneChanged();
}

QColor Controller::zoneColor(int zoneIndex) const {
    switch (zoneIndex) {
        case 1: return zone1Color_;
        case 2: return zone2Color_;
        case 3: return zone3Color_;
        case 4: return zone4Color_;
        default: return currentColor();
    }
}

void Controller::setZoneColor(int zoneIndex, const QColor& color) {
    switch (zoneIndex) {
        case 1: zone1Color_ = color; emit zone1ColorChanged(); break;
        case 2: zone2Color_ = color; emit zone2ColorChanged(); break;
        case 3: zone3Color_ = color; emit zone3ColorChanged(); break;
        case 4: zone4Color_ = color; emit zone4ColorChanged(); break;
        default: break;
    }
    saveConfig();
}

void Controller::setZone1Color(const QColor& value) { setZoneColor(1, value); }
void Controller::setZone2Color(const QColor& value) { setZoneColor(2, value); }
void Controller::setZone3Color(const QColor& value) { setZoneColor(3, value); }
void Controller::setZone4Color(const QColor& value) { setZoneColor(4, value); }

void Controller::setStatus(const QString& value) {
    if (status_ == value) return;
    status_ = value;
    emit statusChanged();
}

void Controller::setHexText(const QString& value) {
    if (hexText_ == value) return;
    hexText_ = value;
    emit hexTextChanged();
}

void Controller::colorChanged(int hue, float saturation, float value) {
    hue_ = hue;
    saturation_ = saturation;
    value_ = value;
    emit hueChanged();
    emit saturationChanged();
    emit valueChanged();
    emit currentColorChanged();

    auto color = currentColor();
    setHexText(to_color(color).to_hex().c_str());

    if (zonesLinked_) {
        setZoneColor(1, color);
        setZoneColor(2, color);
        setZoneColor(3, color);
        setZoneColor(4, color);
    } else {
        setZoneColor(selectedZone_, color);
    }
}

void Controller::pickZone(int zoneIndex) {
    setSelectedZone(zoneIndex);
    auto color = to_color(zoneColor(zoneIndex));
    auto hsv = rgb_to_hsv(color);
    setHue(static_cast<int>(hsv.hue));
    setSaturation(hsv.saturation);
    setValue(hsv.value);
    setStatus(QStringLiteral("Editing Zone %1").arg(zoneIndex));
}

void Controller::applyKeyboard() {
    auto mode = mode_from_display(mode_);
    auto direction = direction_from_display(direction_);
    auto color = currentRgb();

    KeyboardState state;
    state.mode = mode;
    state.speed = static_cast<uint8_t>(speed_);
    state.brightness = static_cast<uint8_t>(brightness_);
    state.direction = direction;
    state.color = color;
    state.zones_linked = zonesLinked_;

    if (mode == Mode::Static) {
        for (int zoneIndex = 1; zoneIndex <= 4; ++zoneIndex) {
            Color zone_color;
            if (zonesLinked_) {
                zone_color = color;
            } else {
                auto stored = to_color(zoneColor(zoneIndex));
                zone_color = (stored.red == 0 && stored.green == 0 && stored.blue == 0) ? color : stored;
            }
            state.zones.push_back({ZoneId{static_cast<uint8_t>(zoneIndex)}, zone_color});
        }
    }

    auto backend_result = DeviceKeyboardBackend::with_default_paths();
    if (!backend_result) {
        setStatus(QStringLiteral("error"));
        return;
    }

    DeviceKeyboardBackend& backend = *backend_result;
    if (apply(state, backend)) {
        setStatus(QStringLiteral("success"));
    } else {
        setStatus(QStringLiteral("error"));
    }
}

} // namespace predator
