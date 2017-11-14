#include <QLayout>
#include <QInputDialog>

#include "mainwindow.h"
#include "command.h"
# include "notes.h"

/*************************************************************************************************/
                                          /* NOTEVIEWER */
/*************************************************************************************************/

NoteViewer::NoteViewer(NoteVersion &n) : note(&n)
{
    id_l = new QLabel("ID : ");
    title_l = new QLabel("Titre : ");
    crea_l = new QLabel("Date de création : ");
    modif_l = new QLabel("Date de modification : ");
    id_t = new QLineEdit();
    title_t = new QLineEdit();
    crea_t = new QLineEdit();
    modif_t = new QLineEdit();
    id_hbox = new QHBoxLayout();
    title_hbox = new QHBoxLayout();
    crea_hbox = new QHBoxLayout();
    modif_hbox = new QHBoxLayout();
    fenetre_vbox = new QVBoxLayout();

    id_hbox->addWidget(id_l);
    id_hbox->addWidget(id_t);
    title_hbox->addWidget(title_l);
    title_hbox->addWidget(title_t);
    crea_hbox->addWidget(crea_l);
    crea_hbox->addWidget(crea_t);
    modif_hbox->addWidget(modif_l);
    modif_hbox->addWidget(modif_t);

    fenetre_vbox->addLayout(id_hbox);
    fenetre_vbox->addLayout(title_hbox);
    fenetre_vbox->addLayout(crea_hbox);
    fenetre_vbox->addLayout(modif_hbox);

    id_t->setText(n.getId());
    id_t->setReadOnly(true);
    title_t->setText(n.getTitre());
    crea_t->setText(n.getDateCreate().toString("dd.MM.yyyy"));
    crea_t->setReadOnly(true);
    modif_t->setText(n.getDateLastModif().toString("dd.MM.yyyy"));
    modif_t->setReadOnly(true);

    QObject::connect(title_t,SIGNAL(textChanged(QString)),&MainWindow::getMainWindow(),SLOT(enableSave()));

    this->setLayout(fenetre_vbox);
}

/*************************************************************************************************/
                                       /* ARTICLEVIEWER */
/*************************************************************************************************/

ArticleViewer::ArticleViewer(Article& a) : NoteViewer(a)
{
    text_l = new QLabel("Texte : ");
    text_t = new QTextEdit;
    text_hbox = new QHBoxLayout();

    text_hbox->addWidget(text_l);
    text_hbox->addWidget(text_t);

    fenetre_vbox->addLayout(text_hbox);

    text_t->setText(a.getTexte());

    QObject::connect(text_t,SIGNAL(textChanged()),&MainWindow::getMainWindow(),SLOT(enableSave()));
}

/*************************************************************************************************/
                                         /* TACHEVIEWER */
/*************************************************************************************************/

TacheViewer::TacheViewer(Tache &t) :NoteViewer(t)
{
    action_l = new QLabel("Action : ");
    statut_l = new QLabel("Statut : ");
    deadline_l = new QLabel("Deadline : ");
    prio_l = new QLabel("Priorité : ");

    action_t = new QTextEdit();
    statut_t = new QComboBox();
    deadline_c = new QCheckBox();
    deadline_t = new QDateEdit();
    prio_c = new QCheckBox();
    prio_t = new QSpinBox();
    prio_i = new QLineEdit();

    action_hbox = new QHBoxLayout();
    statut_hbox = new QHBoxLayout();
    deadline_hbox = new QHBoxLayout();
    prio_hbox = new QHBoxLayout();

    action_hbox->addWidget(action_l);
    action_hbox->addWidget(action_t);
    statut_hbox->addWidget(statut_l);
    statut_hbox->addWidget(statut_t);
    deadline_hbox->addWidget(deadline_l);
    deadline_hbox->addWidget(deadline_c);
    deadline_hbox->addWidget(deadline_t);
    prio_hbox->addWidget(prio_l);
    prio_hbox->addWidget(prio_c);
    prio_hbox->addWidget(prio_t);
    prio_hbox->addWidget(prio_i);

    fenetre_vbox->addLayout(action_hbox);
    fenetre_vbox->addLayout(statut_hbox);
    fenetre_vbox->addLayout(deadline_hbox);
    fenetre_vbox->addLayout(prio_hbox);

    action_t->setText(t.getAction());
    statut_t->addItem("En attente");
    statut_t->addItem("En cours");
    statut_t->addItem("Terminée");
    statutTache s = t.getStatut();
    switch(s)
    {
        case(en_attente) :
            statut_t->setCurrentIndex(0);
        break;

        case(en_cours) :
            statut_t->setCurrentIndex(1);
        break;

        case(terminee) :
            statut_t->setCurrentIndex(2);
        break;
    }

    deadline_c->setChecked(t.getDeadlineBool());
    deadline_t->setEnabled(t.getDeadlineBool());
    deadline_t->setDate(t.getDeadline());
    prio_c->setChecked(t.getPrioriteBool());
    prio_t->setEnabled(t.getPrioriteBool());
    prio_t->setValue(t.getPriorite());

    prio_t->setMinimum(1);
    prio_t->setMaximum(5);

    prio_i->setEnabled(t.getPrioriteBool());
    prio_i->setText(t.getInfo());

    QObject::connect(action_t,SIGNAL(textChanged()),&MainWindow::getMainWindow(),SLOT(enableSave()));
    QObject::connect(statut_t,SIGNAL(currentIndexChanged(int)),&MainWindow::getMainWindow(),SLOT(enableSave()));
    QObject::connect(deadline_c,SIGNAL(toggled(bool)),&MainWindow::getMainWindow(),SLOT(enableSave()));
    QObject::connect(deadline_t,SIGNAL(dateChanged(QDate)),&MainWindow::getMainWindow(),SLOT(enableSave()));
    QObject::connect(prio_c,SIGNAL(toggled(bool)),&MainWindow::getMainWindow(),SLOT(enableSave()));
    QObject::connect(prio_t,SIGNAL(valueChanged(int)),&MainWindow::getMainWindow(),SLOT(enableSave()));
    QObject::connect(prio_i,SIGNAL(textChanged(QString)),&MainWindow::getMainWindow(),SLOT(enableSave()));

    QObject::connect(deadline_c,SIGNAL(toggled(bool)),deadline_t,SLOT(setEnabled(bool)));
    QObject::connect(prio_c,SIGNAL(toggled(bool)),prio_t,SLOT(setEnabled(bool)));
    QObject::connect(prio_c,SIGNAL(toggled(bool)),prio_i,SLOT(setEnabled(bool)));
}

/*************************************************************************************************/
                                    /* MULTIMEDIAVIEWER */
/*************************************************************************************************/

MultimediaViewer::MultimediaViewer(Multimedia &m) :
    NoteViewer(m)
{
    descr_l = new QLabel("Description : ");
    path_l = new QLabel("Chemin du fichier : ");
    img_l = new QLabel("Image : ");
    descr_t = new QTextEdit();
    path_t = new QLineEdit();
    img_t = new QLabel();
    path_b = new QPushButton("Rechercher");
    descr_hbox = new QHBoxLayout();
    path_hbox = new QHBoxLayout();
    img_hbox = new QHBoxLayout();
    QScrollArea* scroll = new QScrollArea();

    img_t->setBackgroundRole(QPalette::Base);
    img_t->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    img_t->setScaledContents(true);

    scroll->setWidget(img_t);



    descr_hbox->addWidget(descr_l);
    descr_hbox->addWidget(descr_t);
    path_hbox->addWidget(path_l);
    path_hbox->addWidget(path_t);
    path_hbox->addWidget(path_b);
    img_hbox->addWidget(img_l);
    img_hbox->addWidget(scroll);

    fenetre_vbox->addLayout(descr_hbox);
    fenetre_vbox->addLayout(path_hbox);
    fenetre_vbox->addLayout(img_hbox);

    descr_t->setText(m.getDescription());
    path_t->setText(m.getImage());
    path_t->setReadOnly(true);
    img_t->setPixmap(QPixmap(m.getImage()));
    img_t->resize(img_t->pixmap()->size());

    QObject::connect(descr_t,SIGNAL(textChanged()),&MainWindow::getMainWindow(),SLOT(enableSave()));
    QObject::connect(path_t,SIGNAL(textChanged(QString)),&MainWindow::getMainWindow(),SLOT(enableSave()));
    QObject::connect(path_b,SIGNAL(clicked()),this,SLOT(loadImage()));
}

void MultimediaViewer::loadImage()
{
    QString fichier = QFileDialog::getOpenFileName(this, "Ouvrir un fichier", QString(), "Images (*.png *.gif *.jpg *.jpeg)");
    path_t->setText(fichier);
    img_t->setPixmap(fichier);
    img_t->resize(img_t->pixmap()->size());
}

ImageViewer::ImageViewer(Image &i) :
    MultimediaViewer(i)
{

}

VideoViewer::VideoViewer(Video &v) :
    MultimediaViewer(v)
{

}

AudioViewer::AudioViewer(Audio &a) :
    MultimediaViewer(a)
{

}

/*************************************************************************************************/
                                       /* CENTRALZONE */
/*************************************************************************************************/

CentralZone::CentralZone()
{
    noteViewer = nullptr;
    layout = new QVBoxLayout;
    setLayout(layout);
}

void CentralZone::addNoteViewer(NoteViewer* nv)
{
    noteViewer = nv;
    layout->addWidget(noteViewer);
}

void CentralZone::removeNoteViewer()
{
    layout->removeWidget(noteViewer);
    delete noteViewer;
    noteViewer = nullptr;
}
