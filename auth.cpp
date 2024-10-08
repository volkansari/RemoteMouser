#include "auth.h"
#include "totpgenerator.h"

Auth::Auth() {}


bool static authenticate(const QString &password) {
    // Kullanıcı adı ve şifre doğrulama işlemi (örneğin, bir veritabanı sorgusu)
    return (password==TOTPGenerator::generateTOTP("deneme"));  // Basit bir örnek
}







