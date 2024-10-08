/*
 * Copyright (c) 2024 Volkan SARI <https://github.com/volkansari>
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

#include "networkhelper.h"
#include <QApplication>
#include <QLabel>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSettings>
#include <QrCodeGenerator.h>
#include <totpgenerator.h>
#include <QSettings>
#include <QFile>
#include <QComboBox>


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow();
    ~MainWindow();
    void loadConfig();


private Q_SLOTS:
    void onGenerateQrCode();
    void saveButton();
    void selectComboboxItem(int id);


private:
    QrCodeGenerator m_generator;
    QTabWidget *m_tabPanel;
    QWidget *m_header;
    QWidget *sm_header;
    QWidget *m_qrcodeScreen;
    QWidget *m_settings;
    QPushButton *m_pushButton;
    QLabel *label_ip;
    QLabel *label_port;
    QLabel *label_privatekey;
    QLabel *label_networkAdapter;
    QLineEdit *lineedit_ip;
    QLineEdit *lineedit_port;
    QLineEdit *lineedit_privatekey;
    QComboBox *combobox_networkAdapter;
    QLabel *m_qrCodeDisplay;
    NetworkHelper *networkHelper;
    QSettings *settings;

private:
    QHostAddress ip;
    int port;
    QString privateKey;
    QList<QHostAddress> networkAdapters;


};
