#pragma once

#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QNetworkReply>
#include <QTimeZone>

class ExchangeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ExchangeWidget(QWidget *parent = nullptr);

private slots:
    void onInputChanged();
    void onCurrencyChanged();
    void onLanguageChanged();
    void onRatesReceived(QNetworkReply *reply);

private:
    void fetchRates();
    void updateResults();
    void retranslateUi();

    enum Lang { ZH, EN };
    Lang m_lang = ZH;

    QLabel *m_langLabel;
    QComboBox *m_langCombo;
    QComboBox *m_currencyCombo;
    QLineEdit *m_amountInput;
    QLabel *m_statusLabel;
    QLabel *m_currencyLabel;
    QLabel *m_amountLabel;
    QGroupBox *m_inputGroup;
    QGroupBox *m_resultGroup;

    QMap<QString, QLabel *> m_resultLabels;
    QMap<QString, double> m_ratesToUSD;

    QNetworkAccessManager *m_network;
    QDateTime m_lastUpdateTime;
    bool m_ratesLoaded = false;
};
