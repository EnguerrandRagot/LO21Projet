#ifndef NEWWIDGETS_H
#define NEWWIDGETS_H

/**
 * \file newwidgets.h
 * \brief Fenêtres secondaires de l'application
 * \version 0.1
 *
 * Ce fichier contient l'ensemble des fenêtres utiles à l'application (hormis MainWindow)
 *
 */

#include <QtWidgets>

/**
 * \class CreateNoteWidget
 * \brief Fenêtre de création de note
 *
 * Ce widget contient tout ce qu'il faut pour créer une nouvelle note.
 *
 */

class CreateNoteWidget : public QWidget {
    Q_OBJECT
private:
    /* Labels */
    QLabel *id_l;
    QLabel *type_l;
    /* Zones de saisies */
    QLineEdit *id_t;
    QComboBox *type_t;
    /* Boutons */
    QPushButton *ok_b;
    QPushButton *cancel_b;
    /* Layouts */
    QHBoxLayout *id_hbox;
    QHBoxLayout *type_hbox;
    QHBoxLayout *buttons_hbox;
    QVBoxLayout *fenetre_vbox;

public:

    /**
     * \brief Constructeur
     *
     * Constructeur de la fenêtre de création de notes.
     *
     */

    CreateNoteWidget();

public slots:

    /**
     * \brief Crée une note
     *
     * La fonction crée une note et l'affiche.
     *
     */

    void createNote();
};

/**
 * \class OpenNoteWidget
 * \brief Fenêtre d'ouverture de note
 *
 * Ce widget contient tout ce qu'il faut pour ouvrir une des notes du NotesManager.
 *
 */

class OpenNoteWidget : public QWidget {
    Q_OBJECT
private:
    /* Labels */
    QLabel *list_l;
    /* Menu déroulant */
    QComboBox *list_t;
    /* Boutons */
    QPushButton *ok_b;
    QPushButton *cancel_b;
    /* Layouts */
    QHBoxLayout *id_hbox;
    QHBoxLayout *buttons_hbox;
    QVBoxLayout *fenetre_vbox;

public:

    /**
     * \brief Constructeur
     *
     * Constructeur de la fenêtre d'ouverture de notes.
     *
     */

    OpenNoteWidget();

public slots:

    /**
     * \brief Ouvre une note
     *
     * Ouvre la note choisie dans la fenêtre.
     *
     */

    void openNote();
};

/**
 * \class CreateRelationsWidget
 * \brief Fenêtre de création de relations
 *
 * Ce widget contient ce qu'il faut pour créer une relation.
 *
 */

class CreateRelationsWidget : public QWidget {
    Q_OBJECT
private:
    /* Labels */
    QLabel *title_l;
    QLabel *descr_l;
    /* Zones de saisies */
    QLineEdit *title_t;
    QLineEdit *descr_t;
    /* Boutons */
    QPushButton *ok_b;
    QPushButton *cancel_b;
    /* Layouts */
    QHBoxLayout *title_hbox;
    QHBoxLayout *descr_hbox;
    QHBoxLayout *buttons_hbox;
    QVBoxLayout *fenetre_vbox;

public:

    /**
     * \brief Constructeur
     *
     * Constructeur de la fenêtre de création des relations.
     *
     */

    CreateRelationsWidget();

public slots:

    /**
     * \brief Crée une relation
     *
     * La fonction crée une relation à partir des infos saisies.
     *
     */

    void createRelation();
};

/**
 * \class AddRelationsWidget
 * \brief Fenêtre d'ajout de couples à une relation
 *
 * Ce widget contient ce qu'il faut pour ajouter un couple de notes à une relation.
 *
 */

class AddRelationsWidget : public QWidget {
    Q_OBJECT
private:
    /* Labels */
    QLabel *noteX_l;
    QLabel *noteY_l;
    QLabel *orien_l;
    /* Listes */
    QComboBox *noteX_c;
    QComboBox *noteY_c;
    QComboBox *add_c;
    QCheckBox *orien_c;
    /* Boutons */
    QPushButton *ok_b;
    QPushButton *cancel_b;
    /* Layouts */
    QHBoxLayout *noteX_hbox;
    QHBoxLayout *noteY_hbox;
    QHBoxLayout *orien_hbox;
    QHBoxLayout *buttons_hbox;
    QVBoxLayout *fenetre_vbox;

public:

    /**
     * \brief Constructeur
     *
     * Constructeur de la fenêtre d'ajout de relations.
     *
     */

    AddRelationsWidget();

public slots:

    /**
     * \brief Ajoute un couple à une relation
     *
     * La fonction ajoute un couple à la relation choisie.
     *
     */

    void addCouple();
};

/**
 * \class ViewRelationsWidget
 * \brief Fenêtre de visualisation d'une relation
 *
 * Ce widget permet de visualiser une relation.
 *
 */

class ViewRelationsWidget : public QWidget {
    Q_OBJECT
private:
    QLabel *title;
    QLabel *descr;
    QListWidget *list;
    QPushButton *ok;
    QVBoxLayout *layout;

public:

    /**
     * \brief Constructeur
     *
     * Constructeur de la fonction de visualisation des relations.
     *
     * \param rel : titre de la relation à visualiser
     *
     */

    ViewRelationsWidget(QString rel);
};

/**
 * \class RelationsWidget
 * \brief Fenêtre de gestion des relations
 *
 * Ce widget fournit l'interface de manipulation des relations.
 *
 */

class RelationsWidget : public QWidget {
    Q_OBJECT
private:
    /* Boutons */
    QPushButton *createRel;
    QPushButton *addCouple;
    QPushButton *viewRel;
    /* Listes */
    QComboBox *view_c;
    /* Layouts */
    QHBoxLayout *create_hbox;
    QHBoxLayout *add_hbox;
    QHBoxLayout *view_hbox;
    QVBoxLayout *fenetre_vbox;

public:

    /**
     * \brief Constructeur
     *
     * Constructeur de la fenêtre de gestion des relations.
     *
     */

    RelationsWidget();

public slots:

    /**
     * \brief Ouvre l'interface de création de relation
     *
     * La fonction crée et affiche l'interface de création de relations.
     *
     */

    void callCreateRelationWindow();

    /**
     * \brief Ouvre l'interface d'ajout de couple
     *
     * La fonction crée et affiche l'interface d'ajout de couple.
     *
     */

    void callAddCoupleWindow();

    /**
     * \brief Ouvre l'interface de visualisation des relations
     *
     * La fonction crée et affiche l'interface de visualisation des relations.
     *
     */

    void callViewRelationWindow();
};

#endif // NEWWIDGETS_H
