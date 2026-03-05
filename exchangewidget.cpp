#include "exchangewidget.h"

#include <QHBoxLayout>
#include <QJsonObject>

const QStringList m_currencies{"USD", "EUR", "AED", "CNY", "HKD", "TWD"};

ExchangeWidget::ExchangeWidget(QWidget *parent) : QWidget(parent), m_network(new QNetworkAccessManager(this))
{
	setMinimumWidth(450);

	auto *mainLayout = new QVBoxLayout(this);
	mainLayout->setSpacing(12);
	mainLayout->setContentsMargins(20, 20, 20, 20);

	auto *langLayout = new QHBoxLayout;
	m_langLabel = new QLabel;
	langLayout->addWidget(m_langLabel);
	m_langCombo = new QComboBox;
	m_langCombo->addItems({"简体中文", "English"});
	m_langCombo->setFixedWidth(100);
	langLayout->addWidget(m_langCombo);
	langLayout->addStretch();
	mainLayout->addLayout(langLayout);

	m_inputGroup = new QGroupBox;
	auto *inputLayout = new QGridLayout(m_inputGroup);

	m_currencyLabel = new QLabel;
	inputLayout->addWidget(m_currencyLabel, 0, 0);
	m_currencyCombo = new QComboBox;
	m_currencyCombo->addItems(m_currencies);
	inputLayout->addWidget(m_currencyCombo, 0, 1);

	m_amountLabel = new QLabel;
	inputLayout->addWidget(m_amountLabel, 1, 0);
	m_amountInput = new QLineEdit;
	m_amountInput->setValidator(new QDoubleValidator(0, 1e12, 4, this));
	inputLayout->addWidget(m_amountInput, 1, 1);

	mainLayout->addWidget(m_inputGroup);

	m_resultGroup = new QGroupBox;
	auto *resultLayout = new QGridLayout(m_resultGroup);

	QFont resultFont;
	resultFont.setPointSize(14);
	resultFont.setBold(true);

	for (int i = 0; i < m_currencies.size(); ++i)
	{
		const auto &cur = m_currencies[i];
		auto *nameLabel = new QLabel(cur);
		nameLabel->setFont(resultFont);
		resultLayout->addWidget(nameLabel, i, 0);

		auto *valueLabel = new QLabel("—");
		valueLabel->setFont(resultFont);
		valueLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
		resultLayout->addWidget(valueLabel, i, 1);

		m_resultLabels[cur] = valueLabel;
	}

	mainLayout->addWidget(m_resultGroup);

	m_statusLabel = new QLabel;
	m_statusLabel->setAlignment(Qt::AlignCenter);
	mainLayout->addWidget(m_statusLabel);

	connect(m_amountInput, &QLineEdit::textChanged, this, &ExchangeWidget::onInputChanged);
	connect(m_currencyCombo, &QComboBox::currentIndexChanged, this, &ExchangeWidget::onCurrencyChanged);
	connect(m_langCombo, &QComboBox::currentIndexChanged, this, &ExchangeWidget::onLanguageChanged);
	connect(m_network, &QNetworkAccessManager::finished, this, &ExchangeWidget::onRatesReceived);

	m_ratesToUSD["USD"] = 1.00;
	m_ratesToUSD["EUR"] = 0.86;
	m_ratesToUSD["AED"] = 3.67;
	m_ratesToUSD["CNY"] = 6.90;
	m_ratesToUSD["HKD"] = 7.81;

	retranslateUi();
	fetchRates();
}

void ExchangeWidget::retranslateUi()
{
	if (m_lang == ZH)
	{
		setWindowTitle("快速汇率换算");
		m_langLabel->setText("语言:");
		m_inputGroup->setTitle("输入");
		m_currencyLabel->setText("货币:");
		m_amountLabel->setText("金额:");
		m_amountInput->setPlaceholderText("输入金额...");
		m_resultGroup->setTitle("转换结果");

		if (m_ratesLoaded)
			m_statusLabel->setText("汇率更新时间: " + m_lastUpdateTime.toString("yyyy-MM-dd HH:mm:ss"));
		else
			m_statusLabel->setText("正在获取汇率...");
	}
	else
	{
		setWindowTitle("Fast Exchange");
		m_langLabel->setText("Lang:");
		m_inputGroup->setTitle("Input");
		m_currencyLabel->setText("Currency:");
		m_amountLabel->setText("Amount:");
		m_amountInput->setPlaceholderText("Enter amount...");
		m_resultGroup->setTitle("Conversion Results");

		if (m_ratesLoaded)
			m_statusLabel->setText("Rates updated: " + m_lastUpdateTime.toString("yyyy-MM-dd HH:mm:ss"));
		else
			m_statusLabel->setText("Fetching rates...");
	}
}

void ExchangeWidget::fetchRates()
{
	QUrl url("https://open.er-api.com/v6/latest/USD");
	m_network->get(QNetworkRequest(url));
}

void ExchangeWidget::onRatesReceived(QNetworkReply *reply)
{
	if (reply->error() == QNetworkReply::NoError)
	{
		auto doc = QJsonDocument::fromJson(reply->readAll());
		auto rates = doc.object()["rates"].toObject();

		for (const auto &cur: m_currencies)
		{
			if (rates.contains(cur))
			{
				m_ratesToUSD[cur] = rates[cur].toDouble();
			}
		}
		qint64 updateUnix = doc.object()["time_last_update_unix"].toInteger();
		m_lastUpdateTime = QDateTime::fromSecsSinceEpoch(updateUnix, QTimeZone::LocalTime);
		m_ratesLoaded = true;
		retranslateUi();
	}
	else
	{
		if (m_lang == ZH)
			m_statusLabel->setText("获取汇率失败，使用默认汇率");
		else
			m_statusLabel->setText("Failed to fetch rates, using defaults");
	}
	reply->deleteLater();
	updateResults();
}

void ExchangeWidget::onInputChanged() { updateResults(); }

void ExchangeWidget::onCurrencyChanged() { updateResults(); }

void ExchangeWidget::onLanguageChanged()
{
	m_lang = m_langCombo->currentIndex() == 0 ? ZH : EN;
	retranslateUi();
}

void ExchangeWidget::updateResults()
{
	QString text = m_amountInput->text();
	if (text.isEmpty())
	{
		for (auto *label: m_resultLabels)
		{
			label->setText("—");
		}
		return;
	}

	bool ok;
	double amount = text.toDouble(&ok);
	if (!ok) return;

	QString fromCurrency = m_currencyCombo->currentText();
	double fromRate = m_ratesToUSD.value(fromCurrency, 1.0);

	double amountInUSD = amount / fromRate;

	for (const auto &cur: m_currencies)
	{
		double targetRate = m_ratesToUSD.value(cur, 1.0);
		double result = amountInUSD * targetRate;
		m_resultLabels[cur]->setText(QString::number(result, 'f', 6));
	}
}