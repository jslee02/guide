#include "MainWindow.h"

#include "widgets/QOgreWidget.h"

#include <dart/utils/SkelParser.h>

using namespace dart;

//==============================================================================
MainWindow::MainWindow()
{
  //
  mOgreRoot = new Ogre::Root("plugins.cfg");

  //
  setupResources();

  //
  setupRenderSystem();

  //
  createQtWidgets();

  //
  createScene();

  //
  createActions();

  //
  createMenus();

  //
  createContextMenu();

  //
  createToolBars();

  //
  createStatusBar();

  //
  readSettings();

  //
  setWindowIcon(QIcon(":/images/icon.png"));

  //
  setCurrentFile("");
}

//==============================================================================
MainWindow::~MainWindow()
{

}

//==============================================================================
void MainWindow::setupResources(void)
{
  // Load resource paths from config file
  Ogre::ConfigFile cf;
  cf.load("resources.cfg");

  // Go through all sections & settings in the file
  Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

  Ogre::String secName, typeName, archName;
  while (seci.hasMoreElements())
  {
    secName = seci.peekNextKey();
    Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
    Ogre::ConfigFile::SettingsMultiMap::iterator i;
    for (i = settings->begin(); i != settings->end(); ++i)
    {
      typeName = i->first;
      archName = i->second;
      Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName,
                                                                     typeName,
                                                                     secName);
    }
  }
}

//==============================================================================
void MainWindow::setupRenderSystem()
{
  // look for the openGL renderer in Ogre
  Ogre::RenderSystemList::const_iterator availableRendererIt
      = mOgreRoot->getAvailableRenderers().begin();

  while (availableRendererIt != mOgreRoot->getAvailableRenderers().end())
  {
    Ogre::String rName = (*availableRendererIt)->getName();

    if (rName == "OpenGL Rendering Subsystem")
      break;

    ++availableRendererIt;
  }

  if (availableRendererIt == mOgreRoot->getAvailableRenderers().end())
  {
    throw std::runtime_error("We were unable to find the OpenGL renderer in ogre's list, cannot continue");
  }

  // use the OpenGL renderer in the root config
  mRenderSystem = *availableRendererIt;
  mOgreRoot->setRenderSystem(mRenderSystem);
  mRenderWindow = mOgreRoot->initialise(false);
}

//==============================================================================
void MainWindow::createScene()
{
  mSceneManager = mOgreRoot->createSceneManager(Ogre::ST_EXTERIOR_CLOSE);
  mCamera = mSceneManager->createCamera("QOgreWidget_Cam");
  mCamera->setPosition(1.0, 1.0, 200);
  mCamera->setAutoAspectRatio(true);
  //mCameraMan = new QCameraMan(mCamera);
  //mCameraMan->setCamera(mCamera);

  mOgreViewport = mOgreWidget->getEmbeddedOgreWindow()->addViewport(mCamera);

  this->resize(640, 480);
  this->setWindowTitle("QOgreWidget demo");

  // Set the scene's ambient light
  mSceneManager->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));

  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
  // Create an Entity
  Ogre::Entity* ogreHead = mSceneManager->createEntity("Head", "ogrehead.mesh");

  // Create a SceneNode and attach the Entity to it
  Ogre::SceneNode* headNode
      = mSceneManager->getRootSceneNode()->createChildSceneNode("HeadNode");
  headNode->attachObject(ogreHead);

  // Create a Light and set its position
  Ogre::Light* light = mSceneManager->createLight("MainLight");
  light->setPosition(20.0f, 80.0f, 50.0f);
}

//==============================================================================
void MainWindow::createQtWidgets()
{
  //QGroupBox *mainGroup = new QGroupBox;
  //QVBoxLayout *mainLayout = new QVBoxLayout;
  mOgreWidget = new QOgreWidget(mOgreRoot, this);
  //QPushButton *buttonZoomIn = new QPushButton(tr("Zoom &In"));
  //QPushButton *buttonZoomOut = new QPushButton(tr("Zoom &Out"));
  //mainLayout->addWidget(buttonZoomIn);
  //mainLayout->addWidget(buttonZoomOut);
  //connect(buttonZoomIn, SIGNAL(released()), this, SLOT(onZoomIn()));
  //connect(buttonZoomOut, SIGNAL(released()), this, SLOT(onZoomOut()));

  //mainLayout->addWidget(mOgreWidget);
  //mainGroup->setLayout(mainLayout);
  setCentralWidget(mOgreWidget);
}

//==============================================================================
void MainWindow::closeEvent(QCloseEvent *event)
{
  if (okToContinue())
  {
    writeSettings();
    event->accept();
  }
  else
  {
    event->ignore();
  }
}

//==============================================================================
void MainWindow::newFile()
{
  if (okToContinue())
    setCurrentFile("");
}

//==============================================================================
void MainWindow::open()
{
  if (okToContinue())
  {
    QString fileName =
        QFileDialog::getOpenFileName(this, tr("Open DART file"), "",
                                     tr("all files (*.*);;sdf files (*.sdf);;skel files (*.skel);;urdf files (*.urdf)"));

    if (!fileName.isEmpty())
      loadFile(fileName);
  }
}

//==============================================================================
bool MainWindow::save()
{
  if (curFile.isEmpty()) {
    return saveAs();
  } else {
    return saveFile(curFile);
  }
}

//==============================================================================
bool MainWindow::saveAs()
{
  QString fileName = QFileDialog::getSaveFileName(this,
                                                  tr("Save GUIDE"), ".",
                                                  tr("all files (*.*);;sdf files (*.sdf);;skel files (*.skel);;urdf files (*.urdf)"));
  if (fileName.isEmpty())
    return false;

  return saveFile(fileName);
}

//==============================================================================
void MainWindow::about()
{
  QMessageBox::about(this, tr("About GUIDE"),
                     tr("<h2>GUIDE 0.1</h2>"
                        "<p>Copyright &copy; 2013 Software Inc."
                        "<p>GUIDE is a Graphical User Interface for DART Engine."));
}

//==============================================================================
void MainWindow::openRecentFile()
{
  if (okToContinue()) {
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
      loadFile(action->data().toString());
  }
}

//==============================================================================
void MainWindow::updateStatusBar()
{
}

//==============================================================================
void MainWindow::worldStepped()
{
  setWindowModified(true);
  updateStatusBar();
}

//==============================================================================
void MainWindow::createActions()
{
  newAction = new QAction(tr("&New"), this);
  newAction->setIcon(QIcon(":/images/new.png"));
  newAction->setShortcut(QKeySequence::New);
  newAction->setStatusTip(tr("Create a new spreadsheet file"));
  connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));

  openAction = new QAction(tr("&Open..."), this);
  openAction->setIcon(QIcon(":/images/open.png"));
  openAction->setShortcut(QKeySequence::Open);
  openAction->setStatusTip(tr("Open an existing spreadsheet file"));
  connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

  saveAction = new QAction(tr("&Save"), this);
  saveAction->setIcon(QIcon(":/images/save.png"));
  saveAction->setShortcut(QKeySequence::Save);
  saveAction->setStatusTip(tr("Save the spreadsheet to disk"));
  connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

  saveAsAction = new QAction(tr("Save &As..."), this);
  saveAsAction->setStatusTip(tr("Save the spreadsheet under a new "
                                "name"));
  connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

  for (int i = 0; i < MaxRecentFiles; ++i)
  {
    recentFileActions[i] = new QAction(this);
    recentFileActions[i]->setVisible(false);
    connect(recentFileActions[i], SIGNAL(triggered()),
            this, SLOT(openRecentFile()));
  }

  exitAction = new QAction(tr("E&xit"), this);
  exitAction->setShortcut(tr("Ctrl+Q"));
  exitAction->setStatusTip(tr("Exit the application"));
  connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

  aboutAction = new QAction(tr("&About"), this);
  aboutAction->setStatusTip(tr("Show the application's About box"));
  connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

  aboutQtAction = new QAction(tr("About &Qt"), this);
  aboutQtAction->setStatusTip(tr("Show the Qt library's About box"));
  connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

//==============================================================================
void MainWindow::createMenus()
{
  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(newAction);
  fileMenu->addAction(openAction);
  fileMenu->addAction(saveAction);
  fileMenu->addAction(saveAsAction);
  separatorAction = fileMenu->addSeparator();
  for (int i = 0; i < MaxRecentFiles; ++i)
    fileMenu->addAction(recentFileActions[i]);
  fileMenu->addSeparator();
  fileMenu->addAction(exitAction);

  editMenu = menuBar()->addMenu(tr("&Edit"));

  selectSubMenu = editMenu->addMenu(tr("&Select"));

  editMenu->addSeparator();

  toolsMenu = menuBar()->addMenu(tr("&Tools"));

  optionsMenu = menuBar()->addMenu(tr("&Options"));

  menuBar()->addSeparator();

  helpMenu = menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(aboutAction);
  helpMenu->addAction(aboutQtAction);
}

//==============================================================================
void MainWindow::createContextMenu()
{
  //    spreadsheet->addAction(cutAction);
  //    spreadsheet->addAction(copyAction);
  //    spreadsheet->addAction(pasteAction);
  //    spreadsheet->setContextMenuPolicy(Qt::ActionsContextMenu);
}

//==============================================================================
void MainWindow::createToolBars()
{
  fileToolBar = addToolBar(tr("&File"));
  fileToolBar->addAction(newAction);
  fileToolBar->addAction(openAction);
  fileToolBar->addAction(saveAction);

  editToolBar = addToolBar(tr("&Edit"));
  editToolBar->addSeparator();
}

//==============================================================================
void MainWindow::createStatusBar()
{
  locationLabel = new QLabel(" W999 ");
  locationLabel->setAlignment(Qt::AlignHCenter);
  locationLabel->setMinimumSize(locationLabel->sizeHint());

  formulaLabel = new QLabel;
  formulaLabel->setIndent(3);

  statusBar()->addWidget(locationLabel);
  statusBar()->addWidget(formulaLabel, 1);

  //    connect(spreadsheet, SIGNAL(currentCellChanged(int, int, int, int)),
  //            this, SLOT(updateStatusBar()));
  //    connect(spreadsheet, SIGNAL(modified()),
  //            this, SLOT(worldStepped()));

  updateStatusBar();
}

//==============================================================================
void MainWindow::readSettings()
{
  QSettings settings("Software Inc.", "Spreadsheet");

  restoreGeometry(settings.value("geometry").toByteArray());

  recentFiles = settings.value("recentFiles").toStringList();
  updateRecentFileActions();

  //    bool showGrid = settings.value("showGrid", true).toBool();
  //    showGridAction->setChecked(showGrid);

  //    bool autoRecalc = settings.value("autoRecalc", true).toBool();
  //    autoRecalcAction->setChecked(autoRecalc);
}

//==============================================================================
void MainWindow::writeSettings()
{
  QSettings settings("Software Inc.", "Spreadsheet");

  settings.setValue("geometry", saveGeometry());
  settings.setValue("recentFiles", recentFiles);
  //    settings.setValue("showGrid", showGridAction->isChecked());
  //    settings.setValue("autoRecalc", autoRecalcAction->isChecked());
}

//==============================================================================
bool MainWindow::okToContinue()
{
  if (isWindowModified()) {
    int r = QMessageBox::warning(this, tr("Spreadsheet"),
                                 tr("The document has been modified.\n"
                                    "Do you want to save your changes?"),
                                 QMessageBox::Yes | QMessageBox::No
                                 | QMessageBox::Cancel);
    if (r == QMessageBox::Yes) {
      return save();
    } else if (r == QMessageBox::Cancel) {
      return false;
    }
  }
  return true;
}

//==============================================================================
bool MainWindow::loadFile(const QString& fileName)
{
  if (!loadWorld(fileName))
  {
    statusBar()->showMessage(tr("Loading canceled"), 2000);
    return false;
  }

  setCurrentFile(fileName);
  statusBar()->showMessage(tr("File loaded"), 2000);
  return true;
}


//==============================================================================
bool MainWindow::loadWorld(const QString& _fileName)
{
  QFile file(_fileName);

  if (!file.open(QIODevice::ReadOnly))
  {
    QMessageBox::warning(this, tr("GUIDE"),
                         tr("Cannot read file %1:\n%2.")
                         .arg(file.fileName())
                         .arg(file.errorString()));
    return false;
  }

  file.close();

  QApplication::setOverrideCursor(Qt::WaitCursor);

  // TODO: Read other file formats such as sdf, urdf
  std::string stdFileName = _fileName.toStdString();
  simulation::World* world = utils::SkelParser::readWorld(stdFileName);

  QApplication::restoreOverrideCursor();

  if (world == NULL)
  {
    QMessageBox::warning(this, tr("GUIDE"),
                         tr("Cannot read file %1:\n%2.")
                         .arg(file.fileName())
                         .arg(file.errorString()));
    return false;
  }

  mWorld = world;

  return true;
}

//==============================================================================
bool MainWindow::saveFile(const QString &fileName)
{
  //    if (!spreadsheet->writeFile(fileName)) {
  //        statusBar()->showMessage(tr("Saving canceled"), 2000);
  //        return false;
  //    }

  setCurrentFile(fileName);
  statusBar()->showMessage(tr("File saved"), 2000);
  return true;
}

//==============================================================================
void MainWindow::setCurrentFile(const QString &fileName)
{
  curFile = fileName;
  setWindowModified(false);

  QString shownName = tr("Untitled");
  if (!curFile.isEmpty()) {
    shownName = strippedName(curFile);
    recentFiles.removeAll(curFile);
    recentFiles.prepend(curFile);
    updateRecentFileActions();
  }

  setWindowTitle(tr("%1[*] - %2").arg(shownName)
                 .arg(tr("GUIDE")));
}

//==============================================================================
void MainWindow::updateRecentFileActions()
{
  QMutableStringListIterator i(recentFiles);
  while (i.hasNext())
  {
    if (!QFile::exists(i.next()))
      i.remove();
  }

  for (int j = 0; j < MaxRecentFiles; ++j)
  {
    if (j < recentFiles.count())
    {
      QString text = tr("&%1 %2")
                     .arg(j + 1)
                     .arg(strippedName(recentFiles[j]));
      recentFileActions[j]->setText(text);
      recentFileActions[j]->setData(recentFiles[j]);
      recentFileActions[j]->setVisible(true);
    }
    else
    {
      recentFileActions[j]->setVisible(false);
    }
  }

  separatorAction->setVisible(!recentFiles.isEmpty());
}

//==============================================================================
QString MainWindow::strippedName(const QString &fullFileName)
{
  return QFileInfo(fullFileName).fileName();
}

