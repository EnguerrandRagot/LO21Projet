#define _GLIBCXX_USE_CXX11_ABI 0

#include <QLayout>
#include <QInputDialog>
#include <fstream>

#include "mainwindow.h"
#include "notes.h"
#include "command.h"
#include "viewer.h"

/*************************************************************************************************/
                                          /* MAINWINDOW */
/*************************************************************************************************/

MainWindow::MainWindow()
{
    /* Zone principale de l'application */
    _mainZone = new CentralZone;
    setCentralWidget(_mainZone);

    /* Barre des menus */

    _menuFichier = menuBar()->addMenu("&Fichier");
    _menuEdition = menuBar()->addMenu("&Edition");
    _menuOptions = menuBar()->addMenu("&Options");
    _menuRelations = menuBar()->addMenu("&Relations");

    /* Actions */

    // Action du menu : nouvelle note
    _actionNvlNote = new QAction("&Nouveau", this);
    _menuFichier->addAction(_actionNvlNote);
    QObject::connect(_actionNvlNote,SIGNAL(triggered()),this,SLOT(callCreateNoteWindow()));
    _actionNvlNote->setShortcut(QKeySequence("Ctrl+N"));

    // Action du menu : ouvrir note
    _actionOpenNote = new QAction("&Ouvrir", this);
    _menuFichier->addAction(_actionOpenNote);
    QObject::connect(_actionOpenNote,SIGNAL(triggered()),this,SLOT(callOpenNoteWindow()));
    _actionOpenNote->setShortcut(QKeySequence("Ctrl+O"));

    // Action du menu : sauvegarder note
    _actionSaveNote = new QAction("&Sauvegarder", this);
    _menuFichier->addAction(_actionSaveNote);
    QObject::connect(_actionSaveNote,SIGNAL(triggered()),this,SLOT(saveNote()));
    _actionSaveNote->setShortcut(QKeySequence("Ctrl+S"));

    // Action du menu : fermer note
    _actionCloseNote = new QAction("&Fermer", this);
    _menuFichier->addAction(_actionCloseNote);
    QObject::connect(_actionCloseNote,SIGNAL(triggered()),this,SLOT(closeNote()));
    _actionCloseNote->setShortcut(QKeySequence("Ctrl+K"));

    // Action du menu : quitter l'application
    _actionQuitter = new QAction("&Quitter", this);
    _menuFichier->addAction(_actionQuitter);
    QObject::connect(_actionQuitter,SIGNAL(triggered()),this,SLOT(close()));
    _actionQuitter->setShortcut(QKeySequence("Ctrl+Q"));

    // Action du menu : supprimer une note
    _actionDeleteNote = new QAction("Supp&rimer", this);
    _menuEdition->addAction(_actionDeleteNote);
    QObject::connect(_actionDeleteNote,SIGNAL(triggered()),this,SLOT(deleteNote()));
    _actionDeleteNote->setShortcut(QKeySequence("Ctrl+R"));

    // Action du menu : note précédente
    _precedentNote = new QAction("&Précédent", this);
    _menuEdition->addAction(_precedentNote);
    QObject::connect(_precedentNote,SIGNAL(triggered()),this,SLOT(openPreviousNote()));
    _precedentNote->setShortcut(QKeySequence("Ctrl+P"));

    // Action du menu : note suivante
    _suivantNote = new QAction("S&uivant", this);
    _menuEdition->addAction(_suivantNote);
    QObject::connect(_suivantNote,SIGNAL(triggered()),this,SLOT(openNextNote()));
    _suivantNote->setShortcut(QKeySequence("Ctrl+U"));

    // Action du menu : gérer relations
    _manageRelations = new QAction("&Gérer relations", this);
    _menuRelations->addAction(_manageRelations);
    QObject::connect(_manageRelations,SIGNAL(triggered()),this,SLOT(callRelationsWindow()));
    _suivantNote->setShortcut(QKeySequence("Ctrl+G"));

    /* Options */

    // Option : vider la corbeille auto en fin de programme
    _viderCorbeilleAuto = new QAction("&Vidage corbeille auto", this);
    _menuOptions->addAction(_viderCorbeilleAuto);
    _viderCorbeilleAuto->setCheckable(true);

    /* Barre d'outils */

    _mainTool = addToolBar("mainTB");
    _mainTool->addAction(_actionNvlNote);
    _mainTool->addAction(_actionOpenNote);
    _mainTool->addAction(_actionSaveNote);
    _mainTool->addAction(_actionCloseNote);
    _mainTool->addAction(_actionDeleteNote);

    /* Etat initial des actions */

    _actionCloseNote->setDisabled(true);
    _actionSaveNote->setDisabled(true);
    _actionDeleteNote->setDisabled(true);

    /* Docks */

    // Dock : notes actives dans NotesManager
    _dockActiveNotes = new QDockWidget("Notes actives", this);
    addDockWidget(Qt::LeftDockWidgetArea, _dockActiveNotes);
    _dockActiveNotes->setFeatures(QDockWidget::NoDockWidgetFeatures);

    // Dock : taches actives dans NotesManager
    _dockTaches = new QDockWidget("Taches actives", this);
    addDockWidget(Qt::LeftDockWidgetArea, _dockTaches);
    _dockTaches->setFeatures(QDockWidget::NoDockWidgetFeatures);

    // Dock : taches actives dans NotesManager
    _dockArchivesNotes = new QDockWidget("Archives", this);
    addDockWidget(Qt::LeftDockWidgetArea, _dockArchivesNotes);
    _dockArchivesNotes->setFeatures(QDockWidget::NoDockWidgetFeatures);

    // Liste des notes actives
    _listActiveNotes = new QListWidget();
    _dockActiveNotes->setWidget(_listActiveNotes);
    for(NotesManager::Iterator<Note> it=NotesManager::getManager().beginNote(); it!=NotesManager::getManager().endNote(); ++it)
    {
        if(!it.getCurrent().getIsDelete()) // Notes non archivées
            _listActiveNotes->addItem(it.getCurrent().getId());
    }

    // Liste des taches
    _listActiveTasks = new QListWidget();
    _dockTaches->setWidget(_listActiveTasks);
    for(NotesManager::Iterator<Note> it=NotesManager::getManager().beginNote(); it!=NotesManager::getManager().endNote(); ++it)
    {
        Tache* t = dynamic_cast<Tache*>(it.getCurrent().getCurrent());
        if(t && (t->getDeadlineBool() || t->getPrioriteBool())) // Tache avec prio ou deadline
            _listActiveTasks->addItem(it.getCurrent().getId());
    }

    // Liste des notes archivées
    _listArchivesNotes = new QListWidget();
    _dockArchivesNotes->setWidget(_listArchivesNotes);
    for(NotesManager::Iterator<Note> it=NotesManager::getManager().beginNote(); it!=NotesManager::getManager().endNote(); ++it)
    {
        if(it.getCurrent().getIsDelete()) // Notes archivées
            _listArchivesNotes->addItem(it.getCurrent().getId());
    }

    // Double-cliquer ouvre la note sélectionnée
    QObject::connect(_listActiveNotes, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(openNoteFromActiveList()));
    QObject::connect(_listActiveTasks, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(openNoteFromTasks()));
    QObject::connect(_listArchivesNotes, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(openNoteFromArchives()));
}

/*************************************************************************************************/
                                          /* SINGLETON */
/*************************************************************************************************/

MainWindow::Handler MainWindow::_handler = Handler();

MainWindow& MainWindow::getMainWindow()
{
    if (!_handler._instance) // Si la MainWindow n'a pas encore été instanciée
        _handler._instance = new MainWindow;
    return *_handler._instance;
}

void MainWindow::freeMainWindow()
{
    delete _handler._instance;
    _handler._instance = nullptr;
}

/*************************************************************************************************/
                                            /* DOCKS */
/*************************************************************************************************/

QString MainWindow::getSelectedItemFromActiveNotes()
{
    QListWidgetItem* i = _listActiveNotes->currentItem();
    return i->text();
}

void MainWindow::addItemToListOfNotes(const QString &id)
{
    _listActiveNotes->addItem(id);
}

QString MainWindow::getSelectedItemFromActiveTasks()
{
    QListWidgetItem* i = _listActiveTasks->currentItem();
    return i->text();
}

void MainWindow::addItemToListOfTasks(const QString &id)
{
    _listActiveTasks->addItem(id);
}

QString MainWindow::getSelectedItemFromArchivedNotes()
{
    QListWidgetItem* i = _listArchivesNotes->currentItem();
    return i->text();
}

void MainWindow::addItemToArchives(const QString &id)
{
    _listArchivesNotes->addItem(id);
}

/*************************************************************************************************/
                                     /* FERMETURE MAINWINDOW */
/*************************************************************************************************/

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveOptions();
    closeNote();
    NotesManager::getManager().save();
    NotesManager::freeManager();
    std::cout<<"On ferme l'appli"<<std::endl; //DEBUG
    event->accept();
}

void MainWindow::saveOptions()
{
    std::cout<<"On entre dans la sauvegarde des options";
    std::ofstream config("application.conf");
    if(config)
    {
        /* Sauvegarde des paramètres de la corbeille */
        bool corb = _viderCorbeilleAuto->isChecked();
        config<<"vider_corbeille= "<<corb<<std::endl;

        /* Sauvegarde de la note actuellement affichée */
        std::string curNote;
        if(_mainZone->getNoteViewer())
            curNote = _mainZone->getCurrentNote()->getId().toStdString();
        else
            curNote = "__NULL__";
        config<<"last_note= "<<curNote<<std::endl;
    }
    else
    {
        QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir le fichier de configuration");
    }
}

void MainWindow::loadOptions()
{
    std::ifstream config("application.conf");
    if(config)
    {
        std::string s;
        config>>s;
        bool b;
        config>>b;
        _viderCorbeilleAuto->setChecked(b);
        config>>s;
        config>>s;
        if(s!="__NULL__")
        {
            QString id = QString::fromStdString(s);
            openNote(id);
        }
    }
}

/*************************************************************************************************/
                                           /* SLOTS */
/*************************************************************************************************/

void MainWindow::callCreateNoteWindow()
{
    CreateNoteWidget *cw = new CreateNoteWidget();
    cw->show();
}

void MainWindow::callOpenNoteWindow()
{
    OpenNoteWidget *ow = new OpenNoteWidget();
    ow->show();
}

void MainWindow::callRelationsWindow()
{
    RelationsWidget *rw = new RelationsWidget();
    rw->show();
}

void MainWindow::openNote(QString id)
{
    if (_mainZone->getNoteViewer()) // Si une note est affichée
        closeNote();
    try
    {
        View v;
        NotesManager::getManager().getNote(id).getCurrent()->accept(v);
    }
    catch (NoteException& e)
    {
        QMessageBox::critical(this,"Erreur",e.getInfo());
    }
}

void MainWindow::openNoteFromActiveList()
{
    QListWidgetItem* i = _listActiveNotes->currentItem();
    QString s = i->text();
    openNote(s);
}

void MainWindow::openNoteFromTasks()
{
    openNote(getSelectedItemFromActiveTasks());
}

void MainWindow::openNoteFromArchives()
{
    openNote(getSelectedItemFromArchivedNotes());
}

void MainWindow::openPreviousNote()
{
    if (!_mainZone->getNoteViewer()) // Si une note est affichée
        QMessageBox::critical(this,"Erreur","Aucune note chargée");
    else
    {
        QString id;
        id = _mainZone->getCurrentNote()->getId();
        try
        {
            closeNote();
            View v;
            NotesManager::getManager().getNote(id).getPrevious()->accept(v);
        }
        catch (NoteException& e)
        {
            QMessageBox::critical(this,"Erreur",e.getInfo());
            View v;
            NotesManager::getManager().getNote(id).getVersion()->accept(v);
        }
    }
}

void MainWindow::openNextNote()
{
    if (!_mainZone->getNoteViewer()) // Si une note est affichée
        QMessageBox::critical(this,"Erreur","Aucune note chargée");
    else
    {
        QString id;
        id = _mainZone->getCurrentNote()->getId();
        try
        {
            closeNote();
            View v;
            NotesManager::getManager().getNote(id).getNext()->accept(v);
        }
        catch (NoteException& e)
        {
            QMessageBox::critical(this,"Erreur",e.getInfo());
            NotesManager::getManager().getNote(id).previous();
            View v;
            NotesManager::getManager().getNote(id).getVersion()->accept(v);
        }
    }
}

void MainWindow::saveNote()
{
    std::cout<<"On entre dans la fonction saveNote\n"; //DEBUG
    if(_mainZone->getNoteViewer()) // Si une note est affichée
    {
        try
        {
            Save s;
            _mainZone->getNoteViewer()->accept(s);
            disableSave();
        }
        catch (NoteException e)
        {
            QMessageBox::critical(this,"Erreur",e.getInfo());
        }
    }
}

void MainWindow::closeNote()
{
    std::cout<<"On entre dans la fonction closeNote\n"; //DEBUG
    if (_mainZone->getNoteViewer()) // Si une note est affichée
    {
        try
        {
            _mainZone->removeNoteViewer();
            disableClose();
            disableDelete();
        }
        catch (NoteException e)
        {
            QMessageBox::critical(this,"Erreur",e.getInfo());
        }
    }
}

void MainWindow::deleteNote()
{
    try
    {
        QString id = _mainZone->getNoteViewer()->getCurrentNote()->getId();
        std::cout<<id.toStdString()<<std::endl;
        if(!NotesManager::getManager().getNote(id).getIsDelete())
        {
            closeNote();
            NotesManager::getManager().supprimerNote(id);
            addItemToArchives(id);
        }
        else
            QMessageBox::critical(this,"Erreur","Cette note est déjà archivée !");
    }
    catch (NoteException e)
    {
        QMessageBox::critical(this,"Erreur",e.getInfo());
    }
}
