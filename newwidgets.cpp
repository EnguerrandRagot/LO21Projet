#include "newwidgets.h"
#include "mainwindow.h"

CreateNoteWidget::CreateNoteWidget()
{
    id_l = new QLabel("ID : ");
    type_l = new QLabel("Type : ");

    id_t = new QLineEdit();
    type_t = new QComboBox();

    ok_b = new QPushButton("OK");
    cancel_b = new QPushButton("Annuler");

    id_hbox = new QHBoxLayout();
    type_hbox = new QHBoxLayout();
    buttons_hbox = new QHBoxLayout();
    fenetre_vbox = new QVBoxLayout();

    id_hbox->addWidget(id_l);
    id_hbox->addWidget(id_t);
    type_hbox->addWidget(type_l);
    type_hbox->addWidget(type_t);
    buttons_hbox->addWidget(ok_b);
    buttons_hbox->addWidget(cancel_b);

    fenetre_vbox->addLayout(id_hbox);
    fenetre_vbox->addLayout(type_hbox);
    fenetre_vbox->addLayout(buttons_hbox);

    this->setLayout(fenetre_vbox);

    type_t->addItem("Article");
    type_t->addItem("Tache");
    type_t->addItem("Image");
    type_t->addItem("Audio");
    type_t->addItem("Video");

    QObject::connect(ok_b,SIGNAL(clicked()),this,SLOT(createNote()));
    QObject::connect(cancel_b,SIGNAL(clicked()),this,SLOT(close()));

    this->setWindowModality(Qt::ApplicationModal);
}

void CreateNoteWidget::createNote()
{
    if(id_t->text()!="") {
        try
        {
            QString id = id_t->text();
            NotesManager::getManager().addNote(id,type_t->currentText());
            MainWindow::getMainWindow().addItemToListOfNotes(id);
            MainWindow::getMainWindow().openNote(id);
            close();
        }
        catch (NoteException& e)
        {
            QMessageBox::information(this,"Erreur",e.getInfo());
        }
    }
    else
        QMessageBox::information(this, "Erreur", "Vous devez entrer un id !");
}

OpenNoteWidget::OpenNoteWidget()
{
    list_l = new QLabel("ID : ");

    list_t = new QComboBox();

    ok_b = new QPushButton("OK");
    cancel_b = new QPushButton("Annuler");

    id_hbox = new QHBoxLayout();
    buttons_hbox = new QHBoxLayout();
    fenetre_vbox = new QVBoxLayout();

    id_hbox->addWidget(list_l);
    id_hbox->addWidget(list_t);
    buttons_hbox->addWidget(ok_b);
    buttons_hbox->addWidget(cancel_b);

    fenetre_vbox->addLayout(id_hbox);
    fenetre_vbox->addLayout(buttons_hbox);

    this->setLayout(fenetre_vbox);

    for(NotesManager::Iterator<Note> it=NotesManager::getManager().beginNote(); it!=NotesManager::getManager().endNote(); ++it) {
        list_t->addItem(it.getCurrent().getId());
    }

    QObject::connect(ok_b,SIGNAL(clicked()),this,SLOT(openNote()));
    QObject::connect(cancel_b,SIGNAL(clicked()),this,SLOT(close()));

    this->setWindowModality(Qt::ApplicationModal);
}

void OpenNoteWidget::openNote()
{
    QString s(list_t->currentText());
    MainWindow::getMainWindow().openNote(s);
    close();
}

CreateRelationsWidget::CreateRelationsWidget()
{
    title_l = new QLabel("Titre : ");
    descr_l = new QLabel("Description : ");

    title_t = new QLineEdit();
    descr_t = new QLineEdit();

    ok_b = new QPushButton("OK");
    cancel_b = new QPushButton("Annuler");

    title_hbox = new QHBoxLayout();
    descr_hbox = new QHBoxLayout();
    buttons_hbox = new QHBoxLayout();
    fenetre_vbox = new QVBoxLayout();

    title_hbox->addWidget(title_l);
    title_hbox->addWidget(title_t);
    descr_hbox->addWidget(descr_l);
    descr_hbox->addWidget(descr_t);
    buttons_hbox->addWidget(ok_b);
    buttons_hbox->addWidget(cancel_b);

    fenetre_vbox->addLayout(title_hbox);
    fenetre_vbox->addLayout(descr_hbox);
    fenetre_vbox->addLayout(buttons_hbox);

    this->setLayout(fenetre_vbox);

    QObject::connect(ok_b,SIGNAL(clicked()),this,SLOT(createRelation()));
    QObject::connect(cancel_b,SIGNAL(clicked()),this,SLOT(close()));

    this->setWindowModality(Qt::ApplicationModal);
}

void CreateRelationsWidget::createRelation()
{
    if(title_t->text()!="") {
        try
        {
            QString title = title_t->text();
            QString descr = descr_t->text();
            NotesManager::getManager().addRelation(title,descr);
            close();
        }
        catch (NoteException& e)
        {
            QMessageBox::information(this,"Erreur",e.getInfo());
        }
    }
    else
        QMessageBox::information(this, "Erreur", "Vous devez entrer un titre !");
}

AddRelationsWidget::AddRelationsWidget()
{
    noteX_l = new QLabel("Note 1 : ");
    noteY_l = new QLabel("Note 2 : ");
    orien_l = new QLabel("Orienté de 1 vers 2 ?");

    noteX_c = new QComboBox();
    noteY_c = new QComboBox();
    orien_c = new QCheckBox();
    add_c = new QComboBox();

    ok_b = new QPushButton("OK");
    cancel_b = new QPushButton("Annuler");

    noteX_hbox = new QHBoxLayout();
    noteY_hbox = new QHBoxLayout();
    orien_hbox = new QHBoxLayout();
    buttons_hbox = new QHBoxLayout();
    fenetre_vbox = new QVBoxLayout();

    noteX_hbox->addWidget(noteX_l);
    noteX_hbox->addWidget(noteX_c);
    noteY_hbox->addWidget(noteY_l);
    noteY_hbox->addWidget(noteY_c);
    orien_hbox->addWidget(orien_l);
    orien_hbox->addWidget(orien_c);
    buttons_hbox->addWidget(ok_b);
    buttons_hbox->addWidget(cancel_b);

    fenetre_vbox->addWidget(add_c);
    fenetre_vbox->addLayout(noteX_hbox);
    fenetre_vbox->addLayout(noteY_hbox);
    fenetre_vbox->addLayout(orien_hbox);
    fenetre_vbox->addLayout(buttons_hbox);

    this->setLayout(fenetre_vbox);

    for(NotesManager::Iterator<Relation> it=NotesManager::getManager().beginRelation(); it!=NotesManager::getManager().endRelation(); ++it) {
        add_c->addItem(it.getCurrent().getTitre());
    }

    for(NotesManager::Iterator<Note> it=NotesManager::getManager().beginNote(); it!=NotesManager::getManager().endNote(); ++it) {
        noteX_c->addItem(it.getCurrent().getId());
        noteY_c->addItem(it.getCurrent().getId());
    }

    QObject::connect(ok_b,SIGNAL(clicked()),this,SLOT(addCouple()));
    QObject::connect(cancel_b,SIGNAL(clicked()),this,SLOT(close()));

    this->setWindowModality(Qt::ApplicationModal);
}

void AddRelationsWidget::addCouple()
{

    if(add_c->currentText()!="" && noteX_c->currentText()!="" && noteY_c->currentText()!="")
    {
        try{
            NotesManager::getManager().getRelation(add_c->currentText())->addCouple(noteX_c->currentText(),noteY_c->currentText(),orien_c->isChecked());
            close();
        }
        catch (NoteException& e)
        {
            QMessageBox::information(this,"Erreur",e.getInfo());
        }
    }
    else
        QMessageBox::critical(this, "Erreur", "Vous devez sélectionner une relation et deux notes");

}

ViewRelationsWidget::ViewRelationsWidget(QString rel)
{
    Relation *r = NotesManager::getManager().getRelation(rel);
    title = new QLabel();
    descr = new QLabel();
    list = new QListWidget();
    ok = new QPushButton("OK!");

    layout = new QVBoxLayout();
    layout->addWidget(title);
    layout->addWidget(descr);
    layout->addWidget(list);
    layout->addWidget(ok);

    this->setLayout(layout);

    title->setText(r->getTitre());
    descr->setText(r->getDescription());

    r->first();
    while(!r->isDone())
    {
        Couple *c = r->getCouple();
        QString x = c->getNoteX()->getId();
        QString y = c->getNoteY()->getId();
        QString o;
        if(c->getOrientation())
            o="--->";
        else
            o="<-->";
        list->addItem(x+o+y);
        r->next();
    }

    QObject::connect(ok,SIGNAL(clicked()),this,SLOT(close()));

    this->setWindowModality(Qt::ApplicationModal);
}

RelationsWidget::RelationsWidget()
{
    createRel = new QPushButton("Créer une relation");
    addCouple = new QPushButton("Ajouter un couple à une relation");
    viewRel = new QPushButton("Voir une relation");


    view_c = new QComboBox();

    create_hbox = new QHBoxLayout();
    add_hbox = new QHBoxLayout();
    view_hbox = new QHBoxLayout();
    fenetre_vbox = new QVBoxLayout();

    create_hbox->addWidget(createRel);
    add_hbox->addWidget(addCouple);
    view_hbox->addWidget(viewRel);
    view_hbox->addWidget(view_c);

    fenetre_vbox->addLayout(create_hbox);
    fenetre_vbox->addLayout(add_hbox);
    fenetre_vbox->addLayout(view_hbox);

    this->setLayout(fenetre_vbox);

    for(NotesManager::Iterator<Relation> it=NotesManager::getManager().beginRelation(); it!=NotesManager::getManager().endRelation(); ++it) {
        view_c->addItem(it.getCurrent().getTitre());
    }

    QObject::connect(createRel,SIGNAL(clicked()),this,SLOT(callCreateRelationWindow()));
    QObject::connect(addCouple,SIGNAL(clicked()),this,SLOT(callAddCoupleWindow()));
    QObject::connect(viewRel,SIGNAL(clicked()),this,SLOT(callViewRelationWindow()));

    this->setWindowModality(Qt::ApplicationModal);

}

void RelationsWidget::callCreateRelationWindow()
{
    CreateRelationsWidget* crw = new CreateRelationsWidget();
    crw->show();
}

void RelationsWidget::callAddCoupleWindow()
{
    AddRelationsWidget* arw = new AddRelationsWidget();
    arw->show();
}

void RelationsWidget::callViewRelationWindow()
{
    if(view_c->currentText()!="")
    {
        ViewRelationsWidget* vrw = new ViewRelationsWidget(view_c->currentText());
        vrw->show();
    }
    else
        QMessageBox::critical(this, "Erreur", "Vous devez sélectionner une relation");
}


