#include "mainwindow.h"
#include "command.h"
#include <QFile>
#include <QTextCodec>
#include <QMessageBox>
#include <QLayout>
#include <QInputDialog>
#include <iostream>
#include <vector>


//TODO methode accept pour chaque classe
//Supprimer et corbeille

/****************************************************************************************************************************/
                                                    /* NOTE */
/****************************************************************************************************************************/

NoteVersion* NoteVersion::CreateNote(const QString &id, const QString &type) {
    std::cout<<"CreateNote(id,type) : Entree dans la fonction, id = "<<id.toStdString()<<", type = "<<type.toStdString()<<"\n"; //TODO : A delete, c'est pour le debug
    if(type == "Article") return new Article(id);
    else if (type == "Tache") return new Tache(id);
    else if (type == "Image") return new Image(id);
    else if (type == "Audio") return new Audio(id);
    else if (type == "Video") return new Video(id);
    else throw NoteException("Ce type n'existe pas (cf Note : factory)");
}

NoteVersion* NoteVersion::CreateNote(QXmlStreamReader* streamR) {
    std::cout<<streamR->name().toString().toStdString()<<" <---\n";
    if(streamR->name() == "Article") return new Article(streamR);
    else if (streamR->name() == "Tache") return new Tache(streamR);
    else if (streamR->name() == "Image") return new Image(streamR);
    else if (streamR->name() == "Audio") return new Audio(streamR);
    else if (streamR->name() == "Video") return new Video(streamR);
    else throw NoteException("Ce type n'existe pas (cf Note : factory)");
}

/****************************************************************************************************************************/







/****************************************************************************************************************************/
                                                    /* NOTE MANAGER */
/****************************************************************************************************************************/
NotesManager::Handler NotesManager::_handler=Handler();

NotesManager& NotesManager::getManager() {
    if (!_handler._instance)
        _handler._instance = new NotesManager();
    return *_handler._instance;
}

void NotesManager::freeManager() {
    delete _handler._instance;
    _handler._instance = nullptr;
}






/****************************************************************************************************************************/
                                                    /* GESTION DES NOTES */
/****************************************************************************************************************************/

Note& NotesManager::getNote(const QString& id){
    std::cout<<"On entre dans la fonction getNote\n";
    for(Iterator<Note> it=beginNote(); it!=endNote(); ++it) {
        if (it.getCurrent().getId() == id) return it.getCurrent();
    }

    throw NoteException("Cette Note n'existe pas");
}


// ajoute la note n au tableau "_notes" de NotesManager
void NotesManager::addNote(Note* n){
    std::cout<<"addNote(n) : Entree dans la fonction\n"; //TODO : A delete, c'est pour le debug

    for(Iterator<Note> it=beginNote(); it!=endNote(); ++it) {
        if (it.getCurrent().getId() == n->getId()) throw NoteException("La note existe déjà");
    }

    std::cout<<"addNote(n) : la Note n'existe pas deja, on va l'ajouter\n"; //TODO : A delete, c'est pour le debug
    // si tableau plein ==> agrandissement
    if (_nbNotes==_nbMaxNotes) {
        Note** newNotes = new Note* [_nbMaxNotes+10];
        for (unsigned int i=0; i<_nbMaxNotes; i++) // i<nbNotes ??
            newNotes[i] = _notes[i];

        Note ** oldNotes = _notes;
        _notes = newNotes;
        _nbMaxNotes+=10;

        if (oldNotes)
            delete[] oldNotes;
    }
    // ajout du nouvel élément au tableau
    _notes[_nbNotes++]=n;
    std::cout<<_nbNotes<<std::endl;
}


void NotesManager::addNote(const QString& id, const QString& type){
   std::cout<<"addNote(id,type) : Entree dans la fonction TRUC \n"; //TODO : A delete, c'est pour le debug

    for(Iterator<Note> it=beginNote(); it!=endNote(); ++it) {
        if (it.getCurrent().getId() == id) throw NoteException("La note existe déjà");
    }

    std::cout<<"addNote(id,type) : la Note n'est pas existante, on va la creer \n"; //TODO : A delete, c'est pour le debug
    Note* n = new Note(id,type);
    std::cout<<"addNote(id,type) : On a cree la note "<<n->getCurrent()->getId().toStdString()<<"\n"; //TODO : A delete, c'est pour le debug

    addNote(n);
}


void NotesManager::supprimerNote(const QString& id){
    for(Iterator<Note> it=beginNote(); it!=endNote(); ++it) {
        if (it.getCurrent().getId() == id){
            it.getCurrent().setIsDelete(true);
            supprimerRelationNote(it.getCurrent().getId());
            return;
        }
    }
    throw NoteException("Cette note n'existe pas");

}


void NotesManager::viderCorbeille() {
    Iterator<Note> itCorbeille = Iterator<Note>(_corbeille);
    while (itCorbeille != nullptr) {
        Note* old = itCorbeille.getPointeur();
        ++ itCorbeille;
        delete old;
    }
}

/****************************************************************************************************************************/





/****************************************************************************************************************************/
                                                    /* GESTION DES RELATIONS */
/****************************************************************************************************************************/

void NotesManager::addRelation(Relation* r){
    std::cout<<"addRelation(r*)\n";
    // vérification non égalité des couples entre ceux du tableau _couples de NotesManager et celui passé en paramètre
    for(Iterator<Relation> it=beginRelation(); it!=endRelation(); ++it) {
        if (it.getCurrent().getTitre() == r->getTitre())
            throw "Cette relation est déjà enregistrée";
    }
    // si tableau plein ==> agrandissement
    if (_nbRelations==_nbMaxRelations) {
        Relation** newRelations = new Relation* [_nbMaxRelations+10];
        for (unsigned int i=0; i<_nbMaxRelations; i++)
            newRelations[i] = _relations[i];

        Relation ** oldRelations = _relations;
        _relations = newRelations;
        _nbMaxRelations+=10;

        if (oldRelations)
            delete[] oldRelations;
    }
    // ajout du nouvel élément au tableau
    _relations[_nbRelations++]=r;
}


void NotesManager::addRelation(const QString& titre, const QString& desc){
    for(Iterator<Relation> it=beginRelation(); it!=endRelation(); ++it) {
        if (it.getCurrent().getTitre() == titre)
            throw "Cette relation est déjà enregistrée";
    }
    Relation* r = new Relation(titre, desc);
    addRelation(r);
}


Relation* NotesManager::getRelation(const QString& titre){
    for(Iterator<Relation> it=beginRelation(); it!=endRelation(); ++it) {
        if (it.getCurrent().getTitre() == titre)
            return it.getPointeur();
    }
    throw NoteException("Relation non trouvée");
}


//parcourt le tableau de relations
// pour chaque relation, supprime les couples qui utilisent cette note
void NotesManager::supprimerRelationNote(const QString& id) {
    for(Iterator<Relation> it=beginRelation(); it!=endRelation(); ++it) {
        it.getCurrent().supprimerCouple(id);
    }
}








/****************************************************************************************************************************/
                                                    /* RELATIONS */
/****************************************************************************************************************************/
void Relation::addCouple(const QString& idX, const QString& idY, bool orientation) {
        NotesManager& nM = NotesManager::getManager();
        if(!nM.getNote(idX).getIsDelete() && !nM.getNote(idY).getIsDelete()){
            std::cout<<"Dans le addCouple\n";
            Note& nX= nM.getNote(idX);
            Note& nY= nM.getNote(idY);
            addCouple(&nX, &nY, orientation);
        }
        else throw NoteException("Impossible d'établir une relation avec une note archivée");
}

void Relation::addCouple(Note* nx, Note* ny, bool orientation){
    for(std::vector<Couple*>::iterator it=_couples.begin(); it!=_couples.end(); ++it) {
        if((*it)->getNoteX() == nx && (*it)->getNoteY() == ny && (*it)->getOrientation() == orientation)
            throw "Ce couple est déjà enregistré";
    }

    Couple* c = new Couple(nx, ny, orientation);
    addCouple(c);               // appel à la méthode privée addCouple de la classe relation
}


// supprime tous les couples dans lesquels la note ayant l'attribut _id = id est impliquée
void Relation::supprimerCouple(const QString &id) {
    for (std::vector<Couple*>::iterator it = _couples.begin(); it!=_couples.end(); ++it) {
        if((*it)->getNoteX()->getId() == id || (*it)->getNoteY()->getId() == id) {
            (*it)->setIsDelete(true);
            decNbCouples();
        }
    }

    if (_nbCouples==0)
        setIsDelete(true);
}

void Relation::supprimerCouple(Couple* c) {
    for (std::vector<Couple*>::iterator it=_couples.begin(); it!=_couples.end(); it++)
        if((*it) == c){
            (*it)->setIsDelete(true);
            decNbCouples();
        }
    if (_nbCouples==0)
        setIsDelete(true);
}








/****************************************************************************************************************************/













/****************************************************************************************************************************/



/****************************************************************************************************************************/
                                                    /* SAUVEGARDE XML */
/****************************************************************************************************************************/


void NotesManager::save() {
    //std::cout<<"Sauvegarde";
    std::cout<<QString::number(_nbNotes).toStdString()<<"---------------\n";
    QFile newfile(filename);
    if (!newfile.open(QIODevice::WriteOnly |QIODevice::Truncate | QIODevice::Text))
        throw NoteException(QString("Erreur sauvegarde notes : ouverture fichier xml"));
    streamWXML = new QXmlStreamWriter(&newfile);
    streamWXML->setAutoFormatting(true);

    streamWXML->writeStartDocument();
    streamWXML->writeStartElement("NotesManager");



    //Sauvegarde des notes
        //streamWXML->writeStartElement("Notes");
            SaveXML sx;
            for(Iterator<Note> it=beginNote(); it!=endNote(); ++it) {
                if(!it.getCurrent().getIsDelete()){
                    streamWXML->writeStartElement("Note");
                            streamWXML->writeAttribute("Id",it.getCurrent().getId());
                            it.getCurrent().first();
                            while(!it.getCurrent().isDone()){
                                it.getCurrent().getVersion()->accept(sx);
                                it.getCurrent().next();
                            }
                    streamWXML->writeEndElement();
                }
                //TODO : destruction des Notes
            }
        //streamWXML->writeEndElement();

    std::cout<<"Sauvegarde des notes effectuée\n";


    //Sauvegarde des relations
        //streamWXML->writeStartElement("Relations");
            for(Iterator<Relation> it=beginRelation(); it!=endRelation(); ++it) {
                if(!it.getCurrent().getIsDelete()){
                 std::cout<<"On entre dans la save des relations\n";
                    streamWXML->writeStartElement("Relation");
                        streamWXML->writeAttribute("Titre",it.getCurrent().getTitre());
                        streamWXML->writeAttribute("Description",it.getCurrent().getDescription());
                                it.getCurrent().first();
                                while(!it.getCurrent().isDone()){
                                    if(!it.getCurrent().getCouple()->isDelete())
                                    {
                                        streamWXML->writeStartElement("Couple");
                                            streamWXML->writeAttribute("NX",it.getCurrent().getCouple()->getNoteX()->getId());
                                            streamWXML->writeAttribute("NY",it.getCurrent().getCouple()->getNoteY()->getId());
                                            streamWXML->writeAttribute("Oriented",QString::number(it.getCurrent().getCouple()->getOrientation()));
                                        streamWXML->writeEndElement();
                                    }
                                    it.getCurrent().next();
                                }
                        streamWXML->writeEndElement();
                        }
                        //TODO : destruction des Notes
                    }
                //streamWXML->writeEndElement();


    streamWXML->writeEndElement();

    streamWXML->writeEndDocument();
    newfile.close();
    std::cout<<"On ferme le fichier\n";
}


/****************************************************************************************************************************/







/****************************************************************************************************************************/
                                                    /* MATRICEADJ */
/****************************************************************************************************************************/


MatriceAdj::MatriceAdj()
{
}


MatriceAdj::~MatriceAdj()
{
    /*for (int i = 0; i < nbSommets; ++i)
        delete[] matrix[i];
    delete[] matrix;
    delete[] Ids;*/

    //TODO : doit-t-on supprimer chaque case? Ou est-ce que c'est fait automatiquement à la fin du programme?
}


void MatriceAdj::ajouterSommet(QString id)
{
    for(unsigned int i=0; i<Ids.size(); i++)
        if(id==Ids[i]) return;

    nbSommets++;

    matrix.resize(nbSommets);

    for(unsigned int i=0; i<Ids.size(); i++)
        matrix[i].resize(nbSommets);

    Ids.push_back(id);

}


void MatriceAdj::retirerSommet(QString id)
{
    for(unsigned int i=0; i<Ids.size(); i++) if(id==Ids[i]){

        nbSommets--;

        matrix.erase(matrix.begin()+i);
        for(unsigned int j=0; j<Ids.size(); j++) matrix[j].erase(matrix[j].begin()+i);

        Ids.erase(Ids.begin()+i);
    }

}


void MatriceAdj::ajouterRelation(QString id1, QString id2, bool oriented)
{
    int i1 = -1;
    int i2 = -1;

    for(unsigned int i=0; i<Ids.size(); i++) if(id1==Ids[i]) i1=i;
    for(unsigned int i=0; i<Ids.size(); i++) if(id2==Ids[i]) i2=i;

    if(i1 != -1 && i2 != -1){
        matrix[i1][i2]++;
        if(!oriented) matrix[i2][i1]++;
    }

}


void MatriceAdj::retirerRelation(QString id1, QString id2, bool oriented)
{
    int i1 = -1;
    int i2 = -1;

    for(unsigned int i=0; i<Ids.size(); i++) if(id1==Ids[i]) i1=i;
    for(unsigned int i=0; i<Ids.size(); i++) if(id2==Ids[i]) i2=i;

    if(i1 != -1 && i2 != -1){
        if(matrix[i1][i2] != 0)matrix[i1][i2]--;
        if(!oriented && matrix[i2][i1] != 0) matrix[i2][i1]++;
    }

}

std::vector<QString> MatriceAdj::successeurs(QString id)
{
    std::vector<QString> succ;

    for(unsigned int i=0; i<Ids.size(); i++) if(id==Ids[i]){
        for(unsigned int j=0; j<Ids.size(); j++) if(matrix[i][j] != 0) succ.push_back(Ids[j]);
    }

    return succ;

}

std::vector<std::vector<QString>> MatriceAdj::arbredescendant(QString id)
{
    std::vector<std::vector<QString>> p; //p est un vecteur contenant des vecteurs d'id
    p.resize(Ids.size());

    std::vector<int> n;
    n.resize(Ids.size());

    int i0 = -1;

    for(unsigned int i=0; i<Ids.size(); i++) {
        p[i].push_back(Ids[i]);

        if(Ids[i] == id) i0 = i;

        for(unsigned int j=0; j<matrix.size(); j++) if(matrix[i][j] != 0) n[i]++;
    }

    // TODO : ERREUR SI id N'EST PAS DANS Ids (if i0 == -1)

    p[i0].push_back(id);

    int i = i0;

    while(n[i0] + abs(i0-i) > 0){
        if(n[i] != 0){
            QString idj = successeurs(Ids[i])[n[i]];
            n[i]--;

            int j = 0;
            for(unsigned int k=0; k<Ids.size(); k++) if(Ids[k] == idj) j=k;

            if(p[j].size()==1){
                p[j].push_back(Ids[i]);
                i = j;
            }
        }
        else for(unsigned int k=0; k<Ids.size(); k++) if(Ids[k] == p[i][0]) i=k;
    }

    return p;

}

/****************************************************************************************************************************/


//TODO : à quoi sert le bool relation?
//void addArticle(const QString& id, const QString& titre = "", const QString& texte = "", const QDate& create = QDate::currentDate(), const QDate& modif = QDate::currentDate()/*, bool relation,*/){
/*    Article* a = new Article(id,titre,texte,create,modif);
    addNote(a);
}*/


// TODO : comment éviter la duplication de code pour les trois méthodes add ???
// utilisation des templates mais comment gérer le fait que le tableau où l'on doit ajouter l'objet est différent selon le paramètre passé à la fonction...
// faire un switch au début de la fonction pour déterminer quel sera le paramètre tab (_notes, _couples, ou _relations) ???


// implémenter les méthodes add avec les arguments permettant de :
// créer l'objet en appelant son constructeur
// l'ajouter au bon tableau d'objets de notesmanager (_notes, _couples, ou _relations).

// améliorer la classe notesmanager avec les méthodes manquantes.
// ajouter l'appel aux méthodes permettant de mettre à jour la matrice dans les méthodes de création d'objets.
