#include "command.h"
#include "notes.h"

/*************************************************************************************************/
                                            /* VIEW */
/*************************************************************************************************/

void View::visit(Article *n)
{
    //std::cout<<"Dans le View d'Article\n";
    MainWindow* mainW = &MainWindow::getMainWindow();
    CentralZone* mainZone = mainW->getCentralZone();
    ArticleViewer* av = new ArticleViewer(*n);
    mainZone->addNoteViewer(av);
    mainW->enableClose();
    mainW->enableDelete();
}

void View::visit(Tache *n)
{
    MainWindow* mainW = &MainWindow::getMainWindow();
    CentralZone* mainZone = mainW->getCentralZone();
    TacheViewer* tv = new TacheViewer(*n);
    mainZone->addNoteViewer(tv);
    mainW->enableClose();
    mainW->enableDelete();
}

void View::visit(Video *n)
{
    MainWindow* mainW = &MainWindow::getMainWindow();
    CentralZone* mainZone = mainW->getCentralZone();
    VideoViewer* vv = new VideoViewer(*n);
    mainZone->addNoteViewer(vv);
    mainW->enableClose();
    mainW->enableDelete();
}

void View::visit(Image *n)
{
    MainWindow* mainW = &MainWindow::getMainWindow();
    CentralZone* mainZone = mainW->getCentralZone();
    ImageViewer* iv = new ImageViewer(*n);
    mainZone->addNoteViewer(iv);
    mainW->enableClose();
    mainW->enableDelete();
}

void View::visit(Audio *n)
{
    MainWindow* mainW = &MainWindow::getMainWindow();
    CentralZone* mainZone = mainW->getCentralZone();
    AudioViewer* av = new AudioViewer(*n);
    mainZone->addNoteViewer(av);
    mainW->enableClose();
    mainW->enableDelete();
}

/*************************************************************************************************/

/*************************************************************************************************/
                                          /* SAVEXML */
/*************************************************************************************************/

void SaveXML::visit(Article *n)
{
    std::cout<<"On entre dans le SaveXML d'Article\n";
    QXmlStreamWriter* stream = NotesManager::getManager().getStreamW();
    stream->writeStartElement("Article");
        stream->writeAttribute("Id",n->getId());
        stream->writeAttribute("DateCreate",n->getDateCreate().toString());
        stream->writeAttribute("DateLastModif",n->getDateLastModif().toString());
        stream->writeAttribute("Titre",n->getTitre());
        stream->writeAttribute("Texte",n->getTexte());
    stream->writeEndElement();
}


void SaveXML::visit(Tache *n)
{
    std::cout<<"On entre dans le SaveXML de Tache\n";
    QXmlStreamWriter* stream = NotesManager::getManager().getStreamW();
    stream->writeStartElement("Tache");
        stream->writeAttribute("Id",n->getId());
        stream->writeAttribute("DateCreate",n->getDateCreate().toString());
        stream->writeAttribute("DateLastModif",n->getDateLastModif().toString());
        stream->writeAttribute("Action",n->getAction());
        stream->writeAttribute("Statut",QString::number((int)(n->getStatut()))); //TODO: Statut
        stream->writeAttribute("BoolPriorite",QString::number(n->getPrioriteBool()));
        stream->writeAttribute("Priorite",QString::number(n->getPriorite()));
        stream->writeAttribute("Info",n->getInfo());
        stream->writeAttribute("DeadLine",n->getDeadline().toString());
        stream->writeAttribute("BoolDeadLine",QString::number(n->getDeadlineBool()));
    stream->writeEndElement();
}


void SaveXML::visit(Video *n)
{
    std::cout<<"On entre dans le SaveXML de Video\n";
    QXmlStreamWriter* stream = NotesManager::getManager().getStreamW();
    stream->writeStartElement("Video");
        stream->writeAttribute("Id",n->getId());
        stream->writeAttribute("DateCreate",n->getDateCreate().toString());
        stream->writeAttribute("DateLastModif",n->getDateLastModif().toString());
        stream->writeAttribute("Description",n->getDescription());
        stream->writeAttribute("Chemin",n->getImage());
    stream->writeEndElement();
}


void SaveXML::visit(Image *n)
{
    std::cout<<"On entre dans le SaveXML d'Image\n";
    QXmlStreamWriter* stream = NotesManager::getManager().getStreamW();
    stream->writeStartElement("Image");
        stream->writeAttribute("Id",n->getId());
        stream->writeAttribute("DateCreate",n->getDateCreate().toString());
        stream->writeAttribute("DateLastModif",n->getDateLastModif().toString());
        stream->writeAttribute("Description",n->getDescription());
        stream->writeAttribute("Chemin",n->getImage());
    stream->writeEndElement();
}


void SaveXML::visit(Audio *n)
{
    std::cout<<"On entre dans le SaveXML d'Audio\n";
    QXmlStreamWriter* stream = NotesManager::getManager().getStreamW();
    stream->writeStartElement("Audio");
        stream->writeAttribute("Id",n->getId());
        stream->writeAttribute("DateCreate",n->getDateCreate().toString());
        stream->writeAttribute("DateLastModif",n->getDateLastModif().toString());
        stream->writeAttribute("Description",n->getDescription());
        stream->writeAttribute("Chemin",n->getImage());
    stream->writeEndElement();
}








/*************************************************************************************************/
                                          /* SAVENOTE */
/*************************************************************************************************/

void Save::visit(ArticleViewer *v)
{
    std::cout<<"On entre dans le Save d'ArticleViewer"<<std::endl;
    QString id = v->getCurrentNote()->getId();
    Note& n = NotesManager::getManager().getNote(id);
    NoteVersion* a = new Article(n.getCurrent()->getId(),v->title_t->text(),n.getCurrent()->getDateCreate(),v->text_t->toPlainText());
    if (a)
    {
        n.addVersion(&a);
        std::cout<<NotesManager::getManager().getNbNote()<<"\n";
    }
    else
    {
        throw NoteException("Erreur lors de la sauvegarde");
    }
}

void Save::visit(TacheViewer *v) //TODO : avoir le statut et la priorité, et la deadline
{
    //std::cout<<"On entre dans le Save de TacheViewer"<<std::endl;
    QString id = v->getCurrentNote()->getId();
    statutTache s;
    QString stat = v->statut_t->currentText();
    if(stat == "En attente")
        s=en_attente;
    else if(stat == "En cours")
        s=en_cours;
    else if(stat == "Terminée")
        s=terminee;
    Note& n = NotesManager::getManager().getNote(id);

    NoteVersion* t = new Tache(n.getCurrent()->getId(),v->title_t->text(), n.getCurrent()->getDateCreate(), v->action_t->toPlainText(),s,v->prio_t->value(), v->prio_i->text(), v->prio_c->isChecked(), v->deadline_t->date(), v->deadline_c->isChecked());
    if (t)
    {
        n.addVersion(&t);
        if(v->prio_c->isChecked() || v->deadline_c->isChecked())
            MainWindow::getMainWindow().addItemToListOfTasks(id);
        //Retirer si tache a plus deadline ou prio
    }
    else
    {
        throw NoteException("Erreur lors de la sauvegarde");
    }
}

void Save::visit(ImageViewer *v)
{
    //std::cout<<"On entre dans le Save d'ImageViewer"<<std::endl;
    QString id = v->getCurrentNote()->getId();
    Note& n = NotesManager::getManager().getNote(id);

    NoteVersion* i = new Image(n.getCurrent()->getId(),v->title_t->text(), n.getCurrent()->getDateCreate(),v->descr_t->toPlainText(),v->path_t->text());

    if (i)
    {
        n.addVersion(&i);
    }
    else
    {
        throw NoteException("Erreur lors de la sauvegarde");
    }
}

void Save::visit(VideoViewer *v)
{
    //std::cout<<"On entre dans le Save de VideoViewer"<<std::endl;
    QString id = v->getCurrentNote()->getId();
    Note& n = NotesManager::getManager().getNote(id);

    NoteVersion* i = new Video(n.getCurrent()->getId(),v->title_t->text(), n.getCurrent()->getDateCreate(),v->descr_t->toPlainText(),v->path_t->text());
    if (i)
    {
        n.addVersion(&i);
    }
    else
    {
        throw NoteException("Erreur lors de la sauvegarde");
    }
}

void Save::visit(AudioViewer *v)
{
    //std::cout<<"On entre dans le Save d'AudioViewer"<<std::endl;
    QString id = v->getCurrentNote()->getId();
    Note& n = NotesManager::getManager().getNote(id);

    NoteVersion* i = new Audio(n.getCurrent()->getId(),v->title_t->text(), n.getCurrent()->getDateCreate(),v->descr_t->toPlainText(),v->path_t->text());
    if (i)
    {
        n.addVersion(&i);
    }
    else
    {
        throw NoteException("Erreur lors de la sauvegarde");
    }
}
