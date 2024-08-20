#include <QApplication>
#include <QtWebView/QtWebView>
#include <QtWebEngineWidgets/QWebEngineView>

namespace vas {

class Html2pdf : public QObject
{
  Q_OBJECT
public:
  explicit Html2pdf(QString file, QString output_path)
    : targetFile(std::move(file))
    , outputPath(std::move(output_path))
    , view(new QWebEngineView)
  {
    connect(view, &QWebEngineView::loadFinished, this, &Html2pdf::loadFinished);
    connect(view, &QWebEngineView::pdfPrintingFinished, this, &Html2pdf::printFinished);
  }
  ~Html2pdf()
  {
    delete view;
  }
  int run() {
    view->load(QUrl::fromUserInput(targetFile));
    return QApplication::exec();
  }
private:
  QString targetFile;
  QString outputPath;
  QWebEngineView* view;

  void loadFinished(bool ok) {
    if(!ok) {
      qDebug() << "failed to load " << targetFile;
      QApplication::exit(1);
      return;
    }
    view->printToPdf(outputPath);
  }
  void printFinished(const QString &filePath, bool ok) {
    if(!ok){
      qDebug() << "failed to write to " << filePath;
      QApplication::exit(1);
    }
    QApplication::quit();
  }
};

}

int main( int argc, char** argv )
{
  QApplication app(argc, argv);
  if(argc==3){
    // usage : ./html2pdf baidu.com ./baidu.pdf
    vas::Html2pdf converter(argv[1], argv[2]);
    return converter.run();
  }
  QApplication::exit(1);
}

#include "main.moc"
