#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>

#include <dart/simulation/World.h>

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  ///
  MainWindow();

  virtual ~MainWindow();

  void setupResources();
  void setupRenderSystem();
  void createScene();
  void createQtWidgets();

protected:
  void closeEvent(QCloseEvent *event);

private slots:
  void newFile();
  void open();
  bool save();
  bool saveAs();
  void about();
  void openRecentFile();
  void updateStatusBar();
  void worldStepped();

private:
  void createActions();
  void createMenus();
  void createContextMenu();
  void createToolBars();
  void createStatusBar();
  void readSettings();
  void writeSettings();
  bool okToContinue();
  bool loadFile(const QString &fileName);
  bool loadWorld(const QString& _fileName);
  bool saveFile(const QString &fileName);
  void setCurrentFile(const QString &fileName);
  void updateRecentFileActions();
  QString strippedName(const QString &fullFileName);

  //----------------------------------------------------------------------------
  // Ogre3d
  //----------------------------------------------------------------------------
  ///
  //QOgreWidget* mOgreWidget;

  ///
  //QCameraMan* mCameraMan;

  //----------------------------------------------------------------------------
  // DART
  //----------------------------------------------------------------------------
  /// World
  dart::simulation::WorldPtr mWorld;

  //----------------------------------------------------------------------------

  QLabel* locationLabel;
  QLabel* formulaLabel;
  QStringList recentFiles;
  QString curFile;

  enum { MaxRecentFiles = 5 };
  QAction* recentFileActions[MaxRecentFiles];
  QAction* separatorAction;

  QMenu *fileMenu;
  QMenu *editMenu;
  QMenu *selectSubMenu;
  QMenu *toolsMenu;
  QMenu *optionsMenu;
  QMenu *helpMenu;
  QToolBar *fileToolBar;
  QToolBar *editToolBar;
  QAction* newAction;
  QAction* openAction;
  QAction* saveAction;
  QAction* saveAsAction;
  QAction* exitAction;
  QAction* aboutAction;
  QAction* aboutQtAction;
};

#endif
