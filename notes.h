#ifndef NOTES_H
#define NOTES_H

/**
 * \file notes.h
 * \brief Gestionnaire de notes
 * \version 0.1
 *
 * Ce fichier contient l'ensemble des classes représentant des notes, ainsi que la classe NotesManager
 *
 */

#include <QString>
#include <QDate>
#include <vector>
#include <iostream>
#include <QFile>
#include <QXmlStreamWriter>



/* TODO :
 * - Utiliser les modif dans les constructeurs
 * - remove note/couple/relation
*/

/****************************************************************************************************************************/
                                                /* NOTESEXCEPTION */
/****************************************************************************************************************************/


/**
 * \class NoteException
 * \brief Classe gérant les exceptions
 *
 * Cette classe permet de gérer toutes les exceptions liées aux notes pour le programme
 */

class NoteException {
    private:
        QString _info;       /**< Information sur l'exception soulevée */
    public:
        /**
         * \brief Constructeur
         * \param message : information sur l'exception
         * Constructeur de la classe NoteException
         */
        NoteException(const QString& message) : _info(message) {}
        /**
         * \brief Récupération du message
         * \return QString contenant le message
         * Méthode qui permet de récupérer le message de l'exception
         */
        const QString& getInfo() {return _info;}
};

/****************************************************************************************************************************/










/****************************************************************************************************************************/
                                                    /* STATUTTACHE */
/****************************************************************************************************************************/

/**
  *\typedef statutTache
  *\enum statutTache : définit les différentes valeurs que peut prendre l'attribut _statut de la classe Tache
  * */

typedef enum {
    en_attente = 1,
    en_cours = 2,
    terminee = 3,
} statutTache;

class Article;
class Tache;
class Image;
class Video;
class Audio;


/****************************************************************************************************************************/










/****************************************************************************************************************************/
                                                    /* NOTEVERSION */
/****************************************************************************************************************************/

/** \class NoteVersion
 *  \brief Classe abstraite représentant une version de Note.
 */

class NoteVersion {
protected:
        QString _id;                    /**< permet d'identifier la note de façon unique. */
        QString _titre;                 /**< contient le titre de la note */
        QDate _dateCreate;              /**< contient la date de création de la note */
        QDate _dateLastModif;           /**< contient la date de dernière modification de la note */

        //! Constructeur de NoteVersion
        /**
         */
        NoteVersion(const QString& id="", const QString& titre="", const QDate& dateCreate=QDate::currentDate())
            : _id(id), _titre(titre), _dateCreate(dateCreate), _dateLastModif(dateCreate){}

        //! Constructeur de NoteVersion
        /** Construit NoteVersion à l'aide du flux de lecture XML.
         */
        NoteVersion(QXmlStreamReader* streamR){
            if (streamR->attributes().hasAttribute("Id")) _id = streamR->attributes().value("Id").toString();
            if (streamR->attributes().hasAttribute("Titre")) _titre = streamR->attributes().value("Titre").toString();
            if (streamR->attributes().hasAttribute("DateCreate")) _dateCreate = QDate::fromString(streamR->attributes().value("DateCreate").toString());
            if (streamR->attributes().hasAttribute("DateLastModif")) _dateLastModif = QDate::fromString(streamR->attributes().value("DateLastModif").toString());
        }


public:
        //! Destructeur
        /**
         */
        virtual ~NoteVersion() {std::cout<<"Entrée dans le destructeur de NoteVersion";}

        // visitor
        virtual void accept(class NoteVisitor &v) =0;

        //factory : pour créer une note
        static NoteVersion* CreateNote(const QString& id, const QString& type);
        static NoteVersion* CreateNote(QXmlStreamReader* streamR);


        //! Accesseur en lecture pour l'attribut _id
        /** \return une référence constante d'une chaîne de caractère contenant la valeur de l'attribut _id
         */
        const QString& getId() const { return _id; }

        //! Accesseur en lecture pour l'attribut _titre
        /** \return une référence constante d'une chaîne de caractère contenant la valeur de l'attribut _titre
         */
        const QString& getTitre() const { return _titre; }

        //! Accesseur en lecture pour l'attribut _id
        /** \return une référence constante de date contenant la valeur de l'attribut _dateCreate
         */
        const QDate& getDateCreate() const { return _dateCreate; }

        //! Accesseur en lecture pour l'attribut _dateLastModif
        /** \return une référence constante d'une chaîne de caractère contenant la valeur de l'attribut _dateLastModif
         */
        const QDate& getDateLastModif() const { return _dateLastModif; }

        //! Accesseur en écriture pour l'attribut _titre
        /** \param une référence constante d'une chaîne de caractère
         */
        void setTitre(const QString& newTitre) { _titre = newTitre; }

        //! Accesseur en écriture pour l'attribut _dateLastModif
        /** \param une référence constante d'une chaîne de caractère
         */
        void setDateLastModif(const QDate& newDateLastModif) { _dateLastModif = newDateLastModif; }
};

/****************************************************************************************************************************/










/****************************************************************************************************************************/
                                                    /* NOTE */
/****************************************************************************************************************************/

/** \class Note
 * \brief Contient l'ensemble des versions d'une note.
 */

class Note{
private :
    QString _id;                                        /**< permet d'identifier une note. (attribut unique et non null) */
    bool _isDelete;                                     /**< permet de savoir si la note doit être sauvegardée ou non à la fermeture de l'application */
    std::vector<NoteVersion*>::iterator _version;       /**< vector contenant les différentes versions d'une note donnée */
    std::vector<NoteVersion*> _notes;                   /**< iterateur sur le vector _version */

public :
    //! Constructeur de Note
    /** Initialise une note avec un identifiant et un type.
     * Les valeurs par défaut sont des chaînes de caractères vides pour les attributs de type QString, "false" pour les booléens et un vector vide pour le tableau _version
     * La reconnaissance de type est effective grâce à la méthode CreateNote de NoteVersion appelée par le constructeur.
    */
    Note(const QString& id = "", const QString& type = "") : _id(id), _isDelete(false), _notes(std::vector<NoteVersion*>()){
        std::cout<<"On est actuellement dans le constructeur de Note\n";
        if(type != "")_notes.push_back(NoteVersion::CreateNote(id,type));
    }

    //! Constructeur de Note
    /** Construit une Note avec un flux de lecture XML.
     */
    Note(const QString& id, QXmlStreamReader* streamR) : _id(id), _isDelete(false), _notes(std::vector<NoteVersion*>()){
        std::cout<<streamR->name().toString().toStdString()<<"\n";
        while(streamR->readNextStartElement()){
            std::cout<<"On est dans le constructeur de Note, on va parser les NoteVersion : "<<streamR->name().toString().toStdString()<<"\n";
            _notes.push_back(NoteVersion::CreateNote(streamR));
            streamR->skipCurrentElement();
        }
    }


    //! Destructeur
    /** Parcourt le tableau de NoteVersion* et détruit les NoteVersion pointées.
     */
    ~Note(){
        for(std::vector<NoteVersion*>::iterator it=_notes.begin();it!=_notes.end();it++) delete *it;
    }

    //! Accesseur en lecture pour l'attribut _id
    /** \return une référence constante d'une chaîne de caractère contenant la valeur de l'attribut _id
     */
    const QString& getId() const {return _id;}

    //! Accesseur en lecture pour l'attribut _isDelete
    /** \return un booléen contenant la valeur de l'attribut _isDelete
     */
    bool getIsDelete() { return _isDelete; }


    //! Accesseur en écriture pour l'attribut _id
    /** \param une référence constante d'une chaîne de caractère
     */
    void setId(const QString& id){_id = id;}

    //! Accesseur en écriture pour l'attribut _isDelete
    /** \param un booleen contenant la nouvelle valeur à donner à l'attribut _isDelete
     */
    void setIsDelete(bool b) { _isDelete=b; }



    //! Fonction d'ajout d'une NoteVersion addVersion
    /** Ajoute la version de Note NoteVersion au début du tableau de NoteVersion et remet l'itérateur sur la version la plus récente.
     */
    void addVersion(NoteVersion** n){
        std::cout<<"Fonction addVersion\n";
        _notes.insert(_notes.begin(),*n);
        _version = _notes.begin();
    }

    //! Fonction d'ajout d'une NoteVersion addNoteVersion
    /** Ajoute la version de Note NoteVersion à la fin du tableau de NoteVersion.
     */
    void addNoteVersion(NoteVersion* nv){
        std::cout<<"addNoteVersion de "<<nv->getId().toStdString()<<"\n";
        _notes.push_back(nv);
    }


    //! Accesseur à la version la plus récente
    /** Remet l'itérateur au début du tableau et renvoit la note correspondante.
     * \return La version de Note la plus récente.
     */
    NoteVersion* getCurrent(){
        std::cout<<"On entre dans la fonction getCurrent\n";
        _version = _notes.begin();
        return (*_version);
    }


    //! Accesseur à la version suivante
    /** Incrémente l'itérateur renvoit la note correspondante.
     * \return La version de Note plus ancienne.
     */
    NoteVersion* getNext(){
        std::cout<<"On entre dans la fonction getNext\n";
        if(++_version == _notes.end()) throw NoteException("Il n'y a pas de version suivante");
        else return (*_version);
    }


    //! Accesseur à la version précédente
    /** Décrémente l'itérateur renvoit la note correspondante.
     * \return La version de Note plus récente.
     */
    NoteVersion* getPrevious(){
        std::cout<<"On entre dans la fonction getPrevious\n";
        if(_version == _notes.begin()) throw NoteException("Il n'y a pas de version précédente");
        else return (*--_version);
    }


    //! Accesseur à la version courante
    /** Renvoit la note correspondant à l'itérateur.
     * \return La version de Note correspondant à l'itérateur.
     */
    NoteVersion* getVersion(){return (*_version);}


    //!
    /** Permet de savoir si l'itérateur est à la fin du tableau.
     * \return Un booléen (vrai si l'itérateur est à la fin du tableau, faux sinon).
     */
    bool isDone(){return _version == _notes.end();}


    //! Réinitialisation de l'itérateur
    /** Remet l'itérateur au début du tableau.
     */
    void first(){ _version = _notes.begin();}


    //! Incrémentation de l'itérateur
    /** Incrémente l'itérateur.
     */
    void next(){_version++;}


    //! Décrémentation de l'itérateur
    /** Décrémente l'itérateur.
     */
    void previous(){_version--;}

};
/****************************************************************************************************************************/










/****************************************************************************************************************************/
                                                    /* NOTE_VISITOR */
/****************************************************************************************************************************/
/** \class NoteVisitor
 * \brief Implémentation du design pattern visitor sur les NoteVersion.
 */
class NoteVisitor {
public:
    virtual void visit(Article *e) = 0;
    virtual void visit(Tache *e) = 0;
    virtual void visit(Video *e) = 0;
    virtual void visit(Image *e) = 0;
    virtual void visit(Audio *e) = 0;
};

/****************************************************************************************************************************/










/****************************************************************************************************************************/
                                                /* ARTICLE */
/****************************************************************************************************************************/
/** \class Article
 * \brief hérite de la classe NoteVersion
 * Décrit une note qui contient en plus des attributs de base, un texte
 */

class Article : public NoteVersion {
    QString _texte;                         /**< attribut qui contient le texte de l'article */

public:
    //! Un constructeur
    /** Initialise un article en appelant le constructeur de NoteVersion avec un identifiant, un titre et la date de création.
     * Puis initialise ses attributs propres (texte). La valeur par défaut est chaîne de caractères vide
     * La reconnaissance de type est effective grâce à la méthode CreateNote de NoteVersion appelée par le constructeur
    */
    Article(const QString& id, const QString& titre ="", const QDate& create = QDate::currentDate(), const QString& texte ="")
        : NoteVersion(id, titre, create), _texte(texte) {std::cout<<"On est dans le constructeur d'Article\n";}


    // doc TODO
    Article(QXmlStreamReader* streamR) : NoteVersion(streamR){
        if (streamR->attributes().hasAttribute("Texte")) _texte = streamR->attributes().value("Texte").toString();
    }


    //! Accesseur en lecture pour l'attribut _texte
    /** \return une référence constante d'une chaîne de caractère contenant la valeur de l'attribut _id
     */
    const QString& getTexte() const { return _texte; }

    //! Accesseur en écriture pour l'attribut _id
    /** \param une référence constante d'une chaîne de caractère
     */
    void setTexte(const QString& newTexte) { _texte = newTexte; }


    // doc TODO
    // visitor
    void accept(NoteVisitor &v) { v.visit(this); }


};

/****************************************************************************************************************************/










/****************************************************************************************************************************/
                                            /* MULTIMEDIA */
/****************************************************************************************************************************/

/** \class Multimedia
 * \brief classe abstraite qui hérite de la classe NoteVersion
 * Décrit une note qui contient en plus des attributs de base, une description et une image (chemin absolu de la localisation du fichier dans la machine)
 */
class Multimedia : public NoteVersion {
protected:
    QString _description;       /**< attribut qui contient la description de l'image */
    QString _image;             /**< attribut qui contient le chemin absolu du fichier dans la machine sur laquelle tourne l'application */

public:
    //! Un constructeur
    /** Initialise une note Multimedia en appelant le constructeur de NoteVersion avec un identifiant, un titre et la date de création. Puis initialise ses attributs propres (_description et _image). Les valeurs par défaut sont des chaînes de caractères vides
    */
    Multimedia(const QString& id=" ", const QString& titre=" ", const QDate& dateCreate=QDate::currentDate(), const QString& desc="", const QString& img="")
        : NoteVersion(id, titre, dateCreate), _description(desc), _image(img) {}


    Multimedia (QXmlStreamReader* streamR) : NoteVersion(streamR){
        if (streamR->attributes().hasAttribute("Description")) _description = streamR->attributes().value("Description").toString();
        if (streamR->attributes().hasAttribute("Chemin")) _image = streamR->attributes().value("Chemin").toString();
    }

    //! Un destructeur
    /** Le destructeur est virtual car la classe Multimedia est abstraite
     */
    virtual ~Multimedia() {}

    // visitor
    virtual void accept(class NoteVisitor &v) =0;


    //! Accesseur en lecture pour l'attribut _description
    /** \return une référence constante d'une chaîne de caractère contenant la valeur de l'attribut _description
     */
    const QString& getDescription() const { return _description; }

    //! Accesseur en lecture pour l'attribut _image
    /** \return une référence constante d'une chaîne de caractère contenant la valeur de l'attribut _image
     */
    const QString& getImage() const { return _image; }


    //! Accesseur en écriture pour l'attribut _description
    /** \param une référence constante d'une chaîne de caractère
     */
    void setDescription(const QString& newDesc) { _description = newDesc; }

    //! Accesseur en écriture pour l'attribut _image
    /** \param une référence constante d'une chaîne de caractère
     */
    void setImage(const QString& newImg) { _image = newImg; }
};

/****************************************************************************************************************************/










/****************************************************************************************************************************/
                                                /* IMAGE - AUDIO - VIDEO */
/****************************************************************************************************************************/
/** \class Image
 * \brief hérite de la classe Multimedia
 * Représente une note Multimedia dont l'image est un fichier d'image.
 * Ne contient pas d'attributs propres
 */
class Image : public Multimedia {
public:
    //! Un constructeur
    /** Initialise une note Image en appelant le constructeur de Multimedia avec un identifiant, un titre et la date de création, une description et une image. Les valeurs par défaut sont des chaînes de caractères vides
    */
    Image(const QString& id="", const QString& titre="", const QDate& dateCreate=QDate::currentDate(), const QString& desc="", const QString& img="")
        : Multimedia(id, titre, dateCreate, desc, img) {}

    Image (QXmlStreamReader* streamR) : Multimedia(streamR){}

    // visitor
    void accept(NoteVisitor &v) { v.visit(this); }
};



/** \class Audio
 * \brief hérite de la classe Multimedia
 * Représente une note Multimedia dont l'image est un fichier audio.
 * Ne contient pas d'attributs propres
 */
class Audio : public Multimedia {
public:
    //! Un constructeur
    /** Initialise une note Audio en appelant le constructeur de Multimedia avec un identifiant, un titre et la date de création, une description et une image. Les valeurs par défaut sont des chaînes de caractères vides
    */
    Audio(const QString& id="", const QString& titre="", const QDate& dateCreate=QDate::currentDate(), const QString& desc="", const QString& img="")
        : Multimedia(id, titre, dateCreate, desc, img) {}

    Audio (QXmlStreamReader* streamR) : Multimedia(streamR){}

    // visitor
    void accept(NoteVisitor &v) { v.visit(this); }
};



/** \class Video
 * \brief hérite de la classe Multimedia
 * Représente une note Multimedia dont l'image est un fichier vidéo.
 * Ne contient pas d'attributs propres
 */
class Video : public Multimedia {
public:
    //! Un constructeur
    /** Initialise une note Video en appelant le constructeur de Multimedia avec un identifiant, un titre et la date de création, une description et une image. Les valeurs par défaut sont des chaînes de caractères vides
    */
    Video(const QString& id="", const QString& titre="", const QDate& dateCreate=QDate::currentDate(), const QString& desc="", const QString& img="")
        : Multimedia(id, titre, dateCreate, desc, img) {}

    Video (QXmlStreamReader* streamR) : Multimedia(streamR){}

    // visitor
    void accept(NoteVisitor &v) { v.visit(this); }
};

/****************************************************************************************************************************/










/****************************************************************************************************************************/
                                                    /* TACHE */
/****************************************************************************************************************************/
/** \class Tache
 * \brief classe qui hérite de la classe NoteVersion
 * Décrit une note dont on peut préciser une action à effectuer et un statut
 */
class Tache  : public NoteVersion {
protected:
    QString _action;                /**< précise l'action à effectuer dont rend compte la note Tache */
    statutTache _statut;            /**< décrit le statut de l'action à effectuer. Valeurs au choix parmi "en_attente", "en_cours" et "terminee" */

    int _priorite;                  /**< permet de préciser une priorité à la tâche. C'est un entier compris entre 1 et 5 */
    QString _info;                  /**< permet de spécifier un libellé  à la priorité (ex : "ça urge !!!") */
    bool _bPriorite;                /**< booléen indiquant si l'attribut _priorité est renseigné ou non */

    QDate _deadline;                /**< permet de spécifier une deadline pour l'action dont rend compte la note Tache */
    bool _bDeadline;                /**< booléen indiquant si l'attribut _deadline est renseigné ou non */

public:
    //! Un constructeur
    /** Initialise une note Tache en appelant le constructeur de NoteVersion avec un identifiant, un titre et la date de création.
     * Puis initialise ses attributs propres. Les valeurs par défaut sont des chaînes de caractères vides pour les attributs de type QString, la date courante pour les attributs de type QDate, false pour les booléens, 0 pour les entiers et en_attente pour l'attribut _statut
    */
    Tache(const QString& id="", const QString& titre="", const QDate& dateCreate=QDate::currentDate(), const QString& action="", statutTache statut=en_attente, int priorite=0, const QString& info=" ", bool prio=false, const QDate& deadline=QDate::currentDate(), bool dline=false)
        : NoteVersion(id, titre, dateCreate), _action(action), _statut(statut), _priorite(priorite), _info(info), _bPriorite(prio), _deadline(deadline), _bDeadline(dline)  {}

    Tache(QXmlStreamReader* streamR) : NoteVersion(streamR){
        if (streamR->attributes().hasAttribute("Statut")) _statut = (statutTache)streamR->attributes().value("Statut").toInt();
        else _statut = en_attente;

        if (streamR->attributes().hasAttribute("Action")) _action = streamR->attributes().value("Action").toString();
        else _action = "";

        if (streamR->attributes().hasAttribute("Priorite")) _priorite = streamR->attributes().value("Priorite").toInt();
        else _priorite = 0;

        if (streamR->attributes().hasAttribute("Info")) _info = streamR->attributes().value("Info").toString();
        else _info = "";

        if (streamR->attributes().hasAttribute("DeadLine")) _deadline = QDate::fromString(streamR->attributes().value("DeadLine").toString());
        else _deadline=QDate::currentDate();

        if (streamR->attributes().hasAttribute("BoolPriorite")) _bPriorite = (bool)streamR->attributes().value("BoolPriorite").toInt();
        else _bPriorite = false;

        if (streamR->attributes().hasAttribute("BoolDeadLine")) _bDeadline = (bool)streamR->attributes().value("BoolDeadLine").toInt();
        else _bDeadline = false;

    }


    //! Accesseur en lecture pour l'attribut _action
    /** \return une référence constante d'une chaîne de caractère contenant la valeur de l'attribut _action
     */
    const QString& getAction() const { return _action; }

    //! Accesseur en lecture pour l'attribut _statut
    /** \return un élément de type "statutTache" contenant la valeur de l'attribut _statut
     */
    statutTache getStatut() const { return _statut; }

    //! Accesseur en lecture pour l'attribut _priorite
    /** \return un entier contenant la valeur de l'attribut _priorite
     */
    int getPriorite() const { return _priorite; }

    //! Accesseur en lecture pour l'attribut _bPriorite
    /** \return une référence constante de booléen contenant la valeur de l'attribut _bPriorite
     */
    const bool& getPrioriteBool() const { return _bPriorite;}

    //! Accesseur en lecture pour l'attribut _info
    /** \return une référence constante d'une chaîne de caractère contenant la valeur de l'attribut _info
     */
    const QString& getInfo() const { return _info; }

    //! Accesseur en lecture pour l'attribut _deadline
    /** \return une référence constante de date contenant la valeur de l'attribut _deadline
     */
    const QDate& getDeadline() const { return _deadline; }

    //! Accesseur en lecture pour l'attribut _bDeadline
    /** \return une référence constante de booléen contenant la valeur de l'attribut _bDeadline
     */
    const bool& getDeadlineBool() const { return _bDeadline;}


    //! Accesseur en écriture pour l'attribut _action
    /** \param une référence constante d'une chaîne de caractère
     */
    void setAction(const QString& newAction) { _action = newAction; }

    //! Accesseur en écriture pour l'attribut _statut
    /** \param une référence constante de statutTache
     */
    void setStatut(const statutTache& newStatut) { _statut = newStatut; }

    //! Accesseur en écriture pour l'attribut _priorite
    /** \param un int
     * La priorite doit être comprise entre 1 et 5
     */
    void setPriorite(int newPriorite) {
    if (newPriorite > 5 || newPriorite < 1)
        throw NoteException ("La priorité doit être entre 1 et 5");
        _priorite = newPriorite;
    }

    //! Accesseur en écriture pour l'attribut _info
    /** \param une référence constante d'une chaîne de caractère
     */
    void setInfo(QString& newInfo) { _info = newInfo; }

    //! Accesseur en écriture pour l'attribut _deadline
    /** \param une référence de date
     */
    void setDeadline(QDate& newDeadline) { _deadline = newDeadline; }



    // visitor
    virtual void accept(NoteVisitor &v) { v.visit(this); }


};

/****************************************************************************************************************************/










/****************************************************************************************************************************/
                                                /* COUPLE */
/****************************************************************************************************************************/
/** \class Couple
 * \brief met en relation deux notes _noteX et _noteY. La relation entre ces deux notes peut être orientée
 */
class Couple {
    Note * _noteX;                  /**< pointeur sur la première note du couple */
    Note * _noteY;                  /**< pointeur sur la deuxième note du couple */
    bool _orientation;              /**< booléen spécifiant si la relation de couple entre ces deux notes est orientée (de _noteX vers _noteY) ou non */
    bool _isDelete;                 /**< booléen indiquant si le couple doit être sauvegardé ou non à la fermeture de l'application */

public:
    //! Un constructeur
    /** Initialise une note Tache en appelant le constructeur de NoteVersion avec un identifiant, un titre et la date de création.
     * Puis initialise ses attributs propres. Les valeurs par défaut sont des chaînes de caractères vides pour les attributs de type QString, la date courante pour les attributs de type QDate, false pour les booléens, 0 pour les entiers et en_attente pour l'attribut _statut
    */
    Couple(Note* nX, Note* nY, bool orientation=false) : _noteX(nX), _noteY(nY), _orientation(orientation), _isDelete(false) {}

    //! Un constructeur de recopie
    /** \param une référence constante de couple
     * Permet de donner aux attributs du couple courant (this), les valeurs des attributs du couple c passé en paramètre.
    */
    Couple(const Couple& c) : _noteX(c._noteX), _noteY(c._noteY), _orientation(c._orientation), _isDelete(c._isDelete) {}

    //! Un destructeur
    /** Appelé lors de la destruction d'un couple
     * Détruit les deux notes qui le constituent
    */
    ~Couple() {}

    //! Un opérateur d'affectation
    /** \param une référence constante de couple
     * permet d'affecter à un couple les valeurs d'un autre couple
     * appeler lors de l'opération this=c
    */
    Couple& operator=(const Couple& c) {
        if (this != dynamic_cast<const Couple*>(&c)) {
            delete _noteX;
            delete _noteY;
            _noteX = c._noteX; //TODO : vérifier s'il faut un new ou pas
            _noteY = c._noteY;
            _orientation = c._orientation;
        }
        return * this;
    }


    //! Accesseur en lecture pour l'attribut _noteX
    /** \return un pointeur de Note
     */
    Note* getNoteX() const { return _noteX; }

    //! Accesseur en lecture pour l'attribut _noteY
    /** \return un pointeur de Note
     */
    Note* getNoteY() const { return _noteY; }

    //! Accesseur en lecture pour l'attribut _orientation
    /** \return un booléen : si sa valeur est "true" alors la relation de couple est orientée de _noteX vers _noteY
     */
    bool getOrientation() const { return _orientation; }
    bool isDelete() const { return _isDelete;}


    //! Accesseur en écriture pour l'attribut _noteX
    /** \param un pointeur de note contenant la nouvelle valeur pour l'attribut _noteX
     */
    void setNoteX(Note* newX) {
        delete _noteX;
        _noteX = newX;
    }

    //! Accesseur en écriture pour l'attribut _noteY
    /** \param un pointeur de note contenant la nouvelle valeur pour l'attribut _noteY
     */
    void setNoteY(Note* newY) {
        delete _noteY;
        _noteY = newY;
    }

    //! Accesseur en écriture pour l'attribut _orientation
    /** \param un booléen contenant la nouvelle valeur pour l'attribut _orientation
     */
    void setOrientation(bool newOrientation) { _orientation = newOrientation; }

    //! Accesseur en écriture pour l'attribut _isDelete
    /** \param un booléen contenant la nouvelle valeur pour l'attribut _isDelete
     */
    void setIsDelete(bool x) { _isDelete=x; }
};

/****************************************************************************************************************************/










/****************************************************************************************************************************/
                                                /* RELATION */
/****************************************************************************************************************************/
/** \class Relation
 * \brief permet de définir des groupes de couples qui sont rattachés au même sujet (renseigné par les attributs _titre et _description)
 * Une relation et les couples qui la constituent sont associés par une relation de composition, leurs cycles de vie sont donc liés.
 * La destruction d'une relation entraîne la destruction des couples qui la constitue
 */
class Relation {
    QString _titre;                                     /**< le titre identifie la relation. Il doit être unique et non NULL */
    QString _description;                               /**< permet de donner des précisions sur la signification de la relation */

    std::vector<Couple*> _couples;                      /**< conteneur C++ vector qui contient tous les couples impliqués dans la relation */
    std::vector<Couple*>::iterator _coupleN;            /**< iterator permettant de parcourir le vector _couples */
    unsigned int _nbCouples;                            /**< indique le nombre de couples présents dans le vector _couples */

    bool _isDelete;                                     /**< permet de savoir si la relation sera ou non sauvegardée à la fermeture de l'application */


    //! Méthode Privée addCouple
    /** \param pointeur de couple
     * Permet d'ajouter le pointeur couple passé en paramètre au vector _couples de la relation
     */
    void addCouple(Couple* c) { _couples.push_back(c); }

public:
    //! Un constructeur
    /** \param titre : référence constante de QString
     * \param desc : une référence constante de QString
     * Les valeurs par défaut des paramètres sont des chaînes de caractères vides
    */
    Relation(const QString& titre = "", const QString& desc ="") : _titre(titre), _description(desc), _couples(std::vector<Couple*>()), _coupleN(_couples.begin()), _nbCouples(0), _isDelete(false) {}

    //! Un constructeur de recopie
    /** \param une référence constante de relation
     * Permet de donner aux attributs de la relation courante (this), les valeurs des attributs de la relation r passée en paramètre.
    */
    Relation (const Relation& r): _titre(r._titre), _description(r._description), _couples(std::vector<Couple*>()), _coupleN(_couples.begin()), _isDelete(r._isDelete) {}

    //! Un destructeur
    /** \brief Appelé lors de la destruction d'une relation. Il se charge de supprimer tous les couples constituant la relation
     */
    ~Relation() {
        for(std::vector<Couple *>::iterator it=_couples.begin(); it!=_couples.end(); it++) {
            delete (*it);
        }
    }


    //! Accesseur en lecture pour l'attribut _titre
    /** \return une référence constante de chaîne de caractères
     */
    const QString& getTitre()const{ return _titre; }

    //! Accesseur en lecture pour l'attribut _description
    /** \return une référence constante de chaîne de caractères
     */
    const QString& getDescription()const{ return _description; }

    //! Accesseur en lecture pour l'attribut _nbCouples
    /** \return un entier contenant le nombre de couples présents dans le vector _couples
     */
    int getNbCouples()const{ return _nbCouples; }

    //! Accesseur en lecture pour l'attribut _isDelete
    /** \return un booléen : si sa valeur est "true" alors la relation ne sera pas sauvegardée à la fermeture de l'application
     */
    bool getIsDelete()const{ return _isDelete;}


    //! Accesseur en écriture pour l'attribut _titre
    /** \param une référence constante de chaîne de caractères contenant la nouvelle valeur pour l'attribut _titre
     */
    void setTitre(const QString& t) { _titre = t; }

    //! Accesseur en écriture pour l'attribut _description
    /** \param une référence constante de chaîne de caractères contenant la nouvelle valeur pour l'attribut _description
     */
    void setDescription(const QString& d) { _description = d; }

    //! Accesseur en écriture pour l'attribut _isDelete
    /** \param un booléen contenant la nouvelle valeur pour l'attribut _isDelete
     */
    void setIsDelete(bool x){ _isDelete = x; }



    //! Méthode permettant d'incrémenter l'attribut _nbCouples
    void incNbCouples(){ _nbCouples++; }

    //! Méthode permettant de décrémenter l'attribut _nbCouples
    void decNbCouples(){ _nbCouples--; }

    //! Méthode permettant de construire un couple et de l'ajouter à la relation
    /** \param un pointeur de note nx
     * \param un pointeur de note ny
     * \param un booléen précisant si le couple est orienté ou non
     */
    void addCouple(Note* nx, Note* ny, bool orientation);

    //! Méthode permettant de construire un couple et de l'ajouter à la relation
    /** \param référence constante de chaîne de caractères contenant l'attribut _id de la première note
     * \param référence constante de chaîne de caractères contenant l'attribut _id de la deuxième note
     */
    void addCouple(const QString& idX, const QString& idY, bool orientation);

    //! Méthode permettant de supprimer un couple d'une relation
    /** \param un pointeur de couple sur le couple à supprimer
     * Recherche le couple passé en paramètre dans la relation et le supprime (le booléen _isDelete du couple prend la valeur true)
     * Si ce couple était le seul de la relation, la valeur du booléen _isDelete de la relation prend la valeur true
     */
    void supprimerCouple(Couple* c);

    //! Méthode permettant de supprimer d'une relation tous les couples dans lesquels la note n est impliquée
    /** \param une référence de chaîne de caractère contenant la valeur de l'attribut _id de la note à supprimer
     */
    void supprimerCouple(const QString& id);

    //! Méthode permettant de supprimer la relation courante
    /** La suppression est effectuée en changeant la valeur de l'attribut _isDelete de la relation à true
     */
    void supprimerRelation() {
        for (std::vector<Couple*>::iterator it=_couples.begin(); it!=_couples.end(); it++)
            (*it)->setIsDelete(true);
        _isDelete = true;
    }



    //! Méthode permettant de récupérer le premier élément du tableau _couples
    void first(){
        _coupleN = _couples.begin();
    }

    //! Méthode permettant de passer à l'élément suivant du tableau _couples
    void next(){
        _coupleN++;
    }

    //! Méthode permettant de savoir si le tableau a été ou non entièrement parcouru
    /** \return booléen indiquant si l'on a entièrement parcouru le tableau
     */
    bool isDone(){
        return _coupleN==_couples.end();
    }

    //! Méthode permettant d'initialiser un pointeur de Couple avec le premier élément du tableau _couples
    Couple* getCouple(){
        return (*_coupleN);
    }

};

/****************************************************************************************************************************/










/****************************************************************************************************************************/
                                                    /* NOTESMANAGER */
/****************************************************************************************************************************/
/** \class NotesManager
 * \brief Gère tout ce qui concerne la création ou la suppression de note ou de relation au sein de l'application
 */
class NotesManager {
private:

    /*** NOTES ***/
    Note** _notes;                                  /**< tableau de pointeurs de note contenant les pointeurs vers toutes les notes de l'application */
    unsigned int _nbNotes;                          /**< entier indiquant le nombre de notes contenues dans le tableau _notes */
    unsigned int _nbMaxNotes;                       /**< entier indiquant la capacité du tableau _notes */
    Note** _corbeille;                              /**< tableau contenant les notes dont le booléen _isDelete a la valeur true */            // TODO : pas sûre...


    /*** RELATIONS ***/
    Relation** _relations;                          /**< tableau de pointeurs de relation contenant les pointeurs vers toutes les relations de l'application */
    unsigned int _nbRelations;                      /**< entier indiquant le nombre de relations contenues dans le tableau _relations */
    unsigned int _nbMaxRelations;                   /**< entier indiquant la capacité du tableau _relations */



    //! Méthode permettant d'ajouter une relation au tableau _relations
    /** \param un pointeur vers la relation à ajouter au tableau
     */
    void addRelation(Relation* r);


    mutable QString filename;                       // pour gérer la sauvegarde dans le xml
    QXmlStreamWriter* streamWXML;


    /** \struct Handler
     * \brief permet d'implémenter le design pattern singleton
     * L'instance unique de NotesManager peut être supprimée par l'utilisateur.
     * Si ce n'est pas le cas, elle sera automatiquement supprimée à la fermeture de l'application
     */
    struct Handler {
        NotesManager* _instance;                    /**< pointeur sur l'unique instance den NotesManager */

        //! Un constructeur
        /** initialise l'instance unique de NotesManager avec un pointeur null
         */
        Handler() : _instance(nullptr) {}

        //! Un destructeur
        /** Désalloue la mémoire associée à l'instance unique de NotesManager
         */
        ~Handler() { delete _instance; }
    };


    static Handler _handler;                        /**< membre statique de la structure handler contenant l'instance unique de NotesManager */

    //! Un constructeur sans arguement privé
    /** NotesManager étant un singleton, son constructeur doit être privé.
     * Les entiers sont initialisés avec la valeur 0 et les pointeurs avec le pointeur nullptr.
     */
    NotesManager() : _notes(nullptr), _nbNotes(0), _nbMaxNotes(0), _relations(nullptr), _nbRelations(0), _nbMaxRelations(0) {}

    //! Un destructeur privé
    /** NotesManager est un singleton : son destructeur est privé
     * détruit toutes les instances des tableaux _notes et _relations
     */
    ~NotesManager(){
        for (Iterator<Note> itNote=beginNote(); itNote!=endNote(); ++itNote) {
            delete itNote.getPointeur();
        }
        for (Iterator<Relation> itRelation=beginRelation(); itRelation!=endRelation(); ++itRelation) {
            delete itRelation.getPointeur();
        }
        delete[] _notes;
        delete[] _relations;
    }

    //! Un constructeur de recopie privé
    /** \param une référence constante de NotesManager
     * NotesManager étant un singleton, son constructeur de recopie doit être privé
     * Le corps de la méthode ne doit pas forcément être défini mais spécifier l'en-tête en privé permet de s'assurer que ce constructeur est privé
     */
    NotesManager(const NotesManager& m);

    //! Un opérateur d'affectation
    /** en-tête de la fonction définie par défaut même si le corps de la fonction n'est pas défini.
     * Permet de s'assurer que cet opérateur est privé
     */
    NotesManager& operator=(const NotesManager& m);

public:

        /*** NOTES ***/

        //! Méthode permettant d'ajouter une note au tableau _notes
        /** \param un pointeur vers la note à ajouter au tableau
         */
        void addNote(Note* n);

        //! Méthode permettant de construire une note et de l'ajouter au tableau _notes de NoteManager
        /** \param une référence constante de chaîne de caractères contentant la valeur de l'attribut _id de la note à construire
         * \param une référence constante de chaîne de caractères contentant la valeur de l'attribut _titre de la note à construire
         * Fait appel à la méthode privée addNote(Note*n) permettant d'ajouter la note nouvellement créée au tableau _notes
         */
        void addNote(const QString& id, const QString& type);

        //! Méthode permettant de supprimer une note du tableau _notes de NotesManager
        /** \param une référence constante de chaîne de caractères contenant la valeur de l'attribut _id de la note à supprimer
         * Fait appel à la méthode NoteManager::supprimerRelationNote qui itère sur toutes les relations du tableau _relations pour trouver tous les couples dans laquelle la note est impliquée
         * et les supprimer des relations correspondantes
         */
        void supprimerNote(const QString& id);

        //! Méthode permettant de retourner la note ayant la valeur id pour son attribut _id
        /** \param référence constante de chaîne de caractères contenant la valeur de l'attribut _id de la note à rechercher
         */
        Note& getNote(const QString& id);

        //! Accesseur en lecture pour l'attribut _nbNotes
        /** \return un entier contenant le nombre de notes contenues dans le tableau _notes
         */
        int getNbNote(){return _nbNotes;}

        //! Méthode permettant d'appeler le destructeur de toutes les notes situées dans le tableau _corbeille
        void viderCorbeille();


        /*** RELATIONS ***/

        //! Méthode permettant de construire une relation et l'ajouter au tableau de relations _relations
        /** \param référence constante de chaîne de caractères contenant la valeur de l'attribut _titre de la relation à créer
         * \param référence constante de chaîne de caractères contenant la valeur de l'attribut _description de la relation à créer
         * Cette méthode initialise la relation avec un couple ou un tableau de couples
         */
        void addRelation(const QString& titre, const QString& desc);

        //! Méthode permettant de supprimer une relation du tableau _relations
        /** \param référence constante de chaîne de caractères contenant le titre de la relation à supprimer
         * Cette méthode appelle la méthode supprimerCouple qui change la valeur du booléen isDelete à true des couples contenus dans la relation
         */
        void supprimerRelation(const QString& titre);

        /** Méthode permettant de supprimer tous les couples contenant la note dont l'attribut _id est id
         * \param référence constante de chaîne de caractère contenant la valeur de l'attribut _id de la note pour laquelle on veut supprimer tous les couples
         * Cette méthode change la valeur du booléen _isDelete de la relation si elle ne contient plus aucun couple
         */
        void supprimerRelationNote(const QString& id);

        /** Méthode permettant d'obtenir un pointeur de relation d'après son titre
         * \param référence constante de chaîne de caractères contenant la valeur de l'attribut _titre de la relation à rechercher
         */
        Relation* getRelation(const QString& titre);



        // accesseurs filename
        QString getFilename() const { return filename; }
        void setFilename(const QString& f) { filename=f; }

        void save();      // save notes in file filename


        QXmlStreamWriter* getStreamW() { return streamWXML; }




        //! Méthode permettant de créer l'instance NoteManager
        static NotesManager& getManager();

        //! Méthode permettant de supprimer l'instance NoteManager
        static void freeManager();


        //! Patron de classe iterator
        /** \param T
         * Permet de générer un iterator quel que soit le tableau de NotesManager que l'on veut parcourir (_notes ou _relations).
         */
        template <typename T>
        class Iterator {
            T ** _current;                       /**< pointeur sur le tableau (_notes ou _relations) */
        public:

            //! Un constructeur
            /** Initialise le pointeur current à 0 par défaut
             */
            Iterator(T** pt=0) : _current(pt) {}


            //! Accesseur en lecture pour l'attribut current
            /** \return un pointeur de pointeur représentant un élément du tableau parcouru
             */
            T& getCurrent()const{ return **_current; }

            //! Accesseur en lecture pour l'attribut current
            /** \return un pointeur sur un élément du tableau parcouru
             */
            T* getPointeur() const{ return *_current;}

            //! Surcharge de l'opérateur ++
            /** \return une référence sur un iterator
             * Permet de passer à l'élément suivant du tableau (incrémenter l'iterator)
             */
            Iterator& operator++() { _current++; return * this; }

            //! Surcharge de l'opérateur --
            /** \return une référence sur un iterator
             * Permet de revenir à l'élément précédent du tableau (décrémenter l'iterator)
             */
            Iterator& operator--() { _current--; return * this; }

            //! Surcharge de l'opérateur d'indirection
            /** \return une référence constante de type T
             * Permet de récupérer la valeur de l'objet pointé par l'iterator
             */
            const T& operator*() const { return **_current; }

            //! Surcharge de l'opérateur d'égalité
            /** \param un iterator
             * \return un booléen
             * Permet de savoir si deux iterator sont égaux
             */
            bool operator==(Iterator it) const { return it._current==_current; }

            //! Surcharge de l'opérateur différent
            /** \param un iterator
             * \return un booléen
             * Permet de savoir si deux iterator sont différents
             */
            bool operator!=(Iterator it) const { return it._current!=_current; }
        };

        //! Méthode permettant d'initialiser un Iterator avec le tableau _notes
        Iterator<Note> beginNote() const { return Iterator<Note>(_notes); }

        //! Méthode permettant d'accéder au premier élément en dehors du tableau de notes (fin + 1)
        Iterator<Note> endNote() const { return Iterator<Note>(_notes+_nbNotes); }


        //! Méthode permettant d'initialiser un Iterator avec le tableau _relations
        Iterator<Relation> beginRelation() const { return Iterator<Relation>(_relations); }

        //! Méthode permettant d'accéder au premier élément en dehors du tableau de relations (fin + 1)
        Iterator<Relation> endRelation() const { return Iterator<Relation>(_relations+_nbRelations); }
};

/****************************************************************************************************************************/










/****************************************************************************************************************************/
                                                    /* MATRICEADJ */
/****************************************************************************************************************************/


class MatriceAdj
{
    private:
        int nbSommets;
        std::vector<std::vector<int>> matrix;
        std::vector<QString> Ids;


    public:
        MatriceAdj();
        ~MatriceAdj();
        void ajouterSommet(QString id);
        void retirerSommet(QString id);
        void ajouterRelation(QString id1, QString id2, bool oriented);
        void retirerRelation(QString id1, QString id2, bool oriented);
        std::vector<QString> successeurs(QString id);
        std::vector<std::vector<QString>> arbredescendant(QString id);

};
/****************************************************************************************************************************/

#endif // NOTES_H
