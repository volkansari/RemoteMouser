/*
 * Copyright (c) 2023 Volkan SARI <https://github.com/volkansari>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "MainWindow.h"


MainWindow::MainWindow():
    networkHelper(new NetworkHelper)
{

    QString filePath = "config.ini"; // Kontrol etmek istediğiniz dosyanın yolu
      settings=new QSettings(filePath, QSettings::IniFormat);
    // QFile ile dosyanın varlığını kontrol et
    QFile file(filePath);
    if (file.exists()) {

        qDebug() << "config.ini dosyası var";

    } else {

        settings->setValue("Network/ip", networkHelper->getActiveNetworkIP().toString());
        settings->setValue("Network/port", 9000);
        settings->setValue("Security/private_key", "volkan");
        qDebug() << filePath << "dosyası mevcut değil.";
        qDebug() << "config.ini dosyası oluşturuldu.";
    }


    // QTabWidget oluştur
    m_tabPanel = new QTabWidget(this);


    // Yeni sekme oluşturmak için bir QWidget


#pragma region QrCodeScreenUI
    /*QrCodeScreen UI*/
    m_qrcodeScreen = new QWidget();
    m_qrCodeDisplay = new QLabel(m_qrcodeScreen);
#pragma region

    /*Setting Screen UI*/
    m_settings = new QWidget();



    // Grid layout oluştur
    QGridLayout *layout = new QGridLayout();

    label_networkAdapter = new QLabel("Ağ Bağdaştırıcısı:");
    combobox_networkAdapter= new QComboBox();

    // QLabel ve QLineEdit ekle
    label_ip = new QLabel(tr("IP") + ": ");
    lineedit_ip = new QLineEdit();

    label_port = new QLabel(tr("Port") + ": ");
    lineedit_port = new QLineEdit();

    label_privatekey = new QLabel("Şifre:");
    lineedit_privatekey = new QLineEdit();

    m_pushButton = new QPushButton(tr("Kaydet"));

    // Layout'a widget'ları yerleştir
    layout->addWidget(label_networkAdapter, 0, 0);    // 0. satır, 0. sütun
    layout->addWidget(combobox_networkAdapter, 0, 1); // 0. satır, 1. sütun

    layout->addWidget(label_ip, 1, 0);    // 1. satır, 0. sütun
    layout->addWidget(lineedit_ip, 1, 1); // 1. satır, 1. sütun

    layout->addWidget(label_port, 2, 0);    // 2. satır, 0. sütun
    layout->addWidget(lineedit_port, 2, 1); // 2. satır, 1. sütun

    layout->addWidget(label_privatekey, 3, 0);    // 3. satır, 0. sütun
    layout->addWidget(lineedit_privatekey, 3, 1); // 3. satır, 1. sütun
    layout->addWidget(m_pushButton, 4, 0); // 4. satır, 1. sütun
    m_settings->setLayout(layout);

    // Butona tıklama sinyalini bağla
    connect(m_pushButton, &QPushButton::clicked, this, &MainWindow::saveButton);
    // Butona tıklama sinyalini bağla
    connect(combobox_networkAdapter, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::selectComboboxItem);

    // TabWidget'e sekme ekle
    m_tabPanel->addTab(m_qrcodeScreen, "QR Code Generator");
    m_tabPanel->addTab(m_settings, "Ayarlar");
    // QTabWidget'i ana pencereye ekleyin
    setCentralWidget(m_tabPanel);

    // Pencere başlığı ayarla
    setWindowTitle(tr("QR Code Generator"));

    onGenerateQrCode();
    loadConfig();
}

MainWindow::~MainWindow() {
    // Belleği serbest bırak
    delete label_ip;
    delete label_port;
    delete label_privatekey;
    delete lineedit_ip;
    delete lineedit_port;
    delete lineedit_privatekey;
    delete m_pushButton;
    delete m_qrCodeDisplay;
    delete m_header;
    delete m_qrcodeScreen;
    delete m_tabPanel;
}

void MainWindow::saveButton() {

    settings->setValue("Network/ip", "192.168.1.100");
    settings->setValue("Network/port", 9000);
    settings->setValue("Security/private_key", "volkan");
}

void MainWindow::selectComboboxItem(int index) {

    QString selectedItem = combobox_networkAdapter->itemText(index);
    lineedit_ip->setText(selectedItem);
    qDebug() << "Selected item index:" << index << ", text:" << selectedItem;
}

void MainWindow::loadConfig() {

    QList<QHostAddress> activeIPs = networkHelper->getActiveIPAddresses();
    if (!activeIPs.isEmpty()) {
    foreach (auto getActiveIPAddress, activeIPs) {

            combobox_networkAdapter->addItem(getActiveIPAddress.toString());

qDebug() << "IP:"+getActiveIPAddress.toString();
       }
    }

    QString ipString = settings->value("Network/ip", "127.0.0.1").toString();
    ip=QHostAddress(ipString);
    lineedit_ip->setText(ip.toString());

    // Port ayarı
    port = settings->value("Network/port", 9000).toInt();
    lineedit_port->setText(QString::number(port));

    // Private key ayarı
    QString privateKey = settings->value("Security/private_key", "volkan").toString();
    lineedit_privatekey->setText(privateKey);
}




void MainWindow::onGenerateQrCode() {

    auto totp=TOTPGenerator::generateTOTP(lineedit_privatekey->text());
    auto qrcodeText=ip.toString()+":"+QString::number(port)+";"+totp;
    // Metinden QR kodu oluştur
    auto image = m_generator.generateQr(qrcodeText);
    QImage scaledImage = image.scaled(200, 200, Qt::KeepAspectRatio, Qt::FastTransformation);

    // Oluşturulan QR kodunu QLabel üzerinde göster
    m_qrCodeDisplay->setPixmap(QPixmap::fromImage(scaledImage));
}
