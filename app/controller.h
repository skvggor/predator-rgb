#pragma once

#include <QColor>
#include <QObject>
#include <QString>

#include "core/keyboard.h"

namespace predator {

class Controller : public QObject {
    Q_OBJECT
    Q_PROPERTY(int hue READ hue WRITE setHue NOTIFY hueChanged)
    Q_PROPERTY(float saturation READ saturation WRITE setSaturation NOTIFY saturationChanged)
    Q_PROPERTY(float value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(QColor currentColor READ currentColor NOTIFY currentColorChanged)
    Q_PROPERTY(QString mode READ mode WRITE setMode NOTIFY modeChanged)
    Q_PROPERTY(int speed READ speed WRITE setSpeed NOTIFY speedChanged)
    Q_PROPERTY(int brightness READ brightness WRITE setBrightness NOTIFY brightnessChanged)
    Q_PROPERTY(QString direction READ direction WRITE setDirection NOTIFY directionChanged)
    Q_PROPERTY(bool zonesLinked READ zonesLinked WRITE setZonesLinked NOTIFY zonesLinkedChanged)
    Q_PROPERTY(int selectedZone READ selectedZone WRITE setSelectedZone NOTIFY selectedZoneChanged)
    Q_PROPERTY(QColor zone1Color READ zone1Color WRITE setZone1Color NOTIFY zone1ColorChanged)
    Q_PROPERTY(QColor zone2Color READ zone2Color WRITE setZone2Color NOTIFY zone2ColorChanged)
    Q_PROPERTY(QColor zone3Color READ zone3Color WRITE setZone3Color NOTIFY zone3ColorChanged)
    Q_PROPERTY(QColor zone4Color READ zone4Color WRITE setZone4Color NOTIFY zone4ColorChanged)
    Q_PROPERTY(QString status READ status WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(QString hexText READ hexText WRITE setHexText NOTIFY hexTextChanged)

public:
    explicit Controller(QObject* parent = nullptr);

    int hue() const { return hue_; }
    void setHue(int value);
    float saturation() const { return saturation_; }
    void setSaturation(float value);
    float value() const { return value_; }
    void setValue(float value);
    QColor currentColor() const;
    QString mode() const { return mode_; }
    void setMode(const QString& value);
    int speed() const { return speed_; }
    void setSpeed(int value);
    int brightness() const { return brightness_; }
    void setBrightness(int value);
    QString direction() const { return direction_; }
    void setDirection(const QString& value);
    bool zonesLinked() const { return zonesLinked_; }
    void setZonesLinked(bool value);
    int selectedZone() const { return selectedZone_; }
    void setSelectedZone(int value);
    QColor zone1Color() const { return zone1Color_; }
    void setZone1Color(const QColor& value);
    QColor zone2Color() const { return zone2Color_; }
    void setZone2Color(const QColor& value);
    QColor zone3Color() const { return zone3Color_; }
    void setZone3Color(const QColor& value);
    QColor zone4Color() const { return zone4Color_; }
    void setZone4Color(const QColor& value);
    QString status() const { return status_; }
    void setStatus(const QString& value);
    QString hexText() const { return hexText_; }
    void setHexText(const QString& value);

    Q_INVOKABLE void colorChanged(int hue, float saturation, float value);
    Q_INVOKABLE void pickZone(int zoneIndex);
    Q_INVOKABLE void applyKeyboard();

signals:
    void hueChanged();
    void saturationChanged();
    void valueChanged();
    void currentColorChanged();
    void modeChanged();
    void speedChanged();
    void brightnessChanged();
    void directionChanged();
    void zonesLinkedChanged();
    void selectedZoneChanged();
    void zone1ColorChanged();
    void zone2ColorChanged();
    void zone3ColorChanged();
    void zone4ColorChanged();
    void statusChanged();
    void hexTextChanged();

private:
    Color currentRgb() const;
    QColor zoneColor(int zoneIndex) const;
    void setZoneColor(int zoneIndex, const QColor& color);
    void saveConfig();
    void loadConfig();
    QString configPath() const;

    int hue_ = 340;
    float saturation_ = 0.76f;
    float value_ = 1.0f;
    QString mode_ = QStringLiteral("Wave");
    int speed_ = 5;
    int brightness_ = 80;
    QString direction_ = QStringLiteral("Left to Right");
    bool zonesLinked_ = true;
    int selectedZone_ = 0;
    QColor zone1Color_;
    QColor zone2Color_;
    QColor zone3Color_;
    QColor zone4Color_;
    QString status_ = QStringLiteral("Ready");
    QString hexText_ = QStringLiteral("#FF3D7E");
};

} // namespace predator
