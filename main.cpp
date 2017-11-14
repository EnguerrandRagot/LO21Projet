#include <QWidget>
#include <iostream>
#include "mainwindow.h"
#include "loadxml.h"

int main(int argc, char *argv[])
{
    QApplication plurinotes(argc, argv);
    std::cout<<"=====> BIENVENUE DANS PLURINOTES 2<=====\n\n"<<std::endl;

    try {
    NotesManager& m = NotesManager::getManager();

    m.setFilename("save.txt");
    XmlNoteManagerReader xmlReader;

        QFile f("save.txt");

        if(!f.open(QFile::ReadOnly | QFile::Text)){
            throw NoteException("Le chargement des donnees ne peut s'effectuer");
        }

        xmlReader.read(&f);
        f.close();
    }
    catch (NoteException e)
    {
         QMessageBox::critical(0, "Erreur", e.getInfo());
    }




    try
    {
    QFile f("save.txt"); //ERREUR si le fichier n'a pas encore été créé
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        throw NoteException(QString("Erreur sauvegarde notes : ouverture fichier xml"));

    QString texte=f.readAll();
    //std::cout<<"**** FICHIER DE SAUVEGARDE ****\n"<<texte.toStdString()<<"\n\n\n";
    f.close();

    //m.addRelation("Référence","Test");

    MainWindow& mainW=MainWindow::getMainWindow();
    mainW.setWindowTitle("PluriNotes");
    mainW.resize(800,600);

    mainW.show();
    mainW.loadOptions();

    //std::cout<<"Manager got\n";
    }
    catch (NoteException e)
    {
        QMessageBox::critical(0, "Erreur", e.getInfo());
    }


    //QObject::connect(&mainW, SIGNAL(close()), &plurinotes, SLOT(quit()));
    return plurinotes.exec();
}
