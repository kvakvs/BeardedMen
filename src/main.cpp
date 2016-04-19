/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2015 David Williams and Matthew Williams

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*******************************************************************************/

#include <QApplication>
#include <QGLFormat>
#include <QTranslator>

#include "ui/main_window.h"

void setup_opengl_profile() {
    QGLFormat gl_fmt;
    gl_fmt.setVersion(3, 2);
    gl_fmt.setProfile(QGLFormat::CoreProfile);
    gl_fmt.setSampleBuffers(true);
    QGLFormat::setDefaultFormat(gl_fmt);
}

std::unique_ptr<QTranslator> setup_translator(QApplication& app) {
    auto translator = std::make_unique<QTranslator>();
    // look up e.g. :/translations/myapp_de.qm
//    if (translator.load(QLocale(QLocale::Russian, QLocale::AnyCountry),
//                        QLatin1String("localization"),
//                        QLatin1String("_"),
//                        QLatin1String(":/lang")))
    if (translator->load(QString("lang/localization_ru.qm")))
    {
        app.installTranslator(translator.get());
    }
    return translator;
}

int main(int argc, char* argv[]) {
    // Create and show the Qt OpenGL window
    QApplication app(argc, argv);
    auto tr = setup_translator(app);

    auto bmaf = QApplication::tr("Bearded men (and a Fortress)");
    app.setApplicationDisplayName(bmaf);
    app.setApplicationName(bmaf);

    setup_opengl_profile();

    auto main_wnd  = new bm::GameMainWindow();
    main_wnd->setWindowTitle(bmaf);
    main_wnd->show();

    // Run the message pump.
    return app.exec();
}
