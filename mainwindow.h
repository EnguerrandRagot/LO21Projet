#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/**
 * \file mainwindow.h
 * \brief Fenêtre principale de l'application
 * \version 0.1
 *
 * Ce fichier contient la classe MainWindow qui représente la fenêtre principale de l'application
 *
 */

#include <QtWidgets>

#include "viewer.h"
#include "newwidgets.h"

/*************************************************************************************************/
                                          /* MAINWINDOW */
/*************************************************************************************************/

/**
 * \class MainWindow
 * \brief Classe gérant la fenêtre principale
 *
 * Cette classe permet de gérer la fenêtre principale du programme
 *
 */

class MainWindow : public QMainWindow {
    Q_OBJECT
private:
    /* Zone principale de l'application */
    CentralZone* _mainZone;

    /* Barre des menus */
    QMenu* _menuFichier;   // Actions basiques de l'application
    QMenu* _menuEdition;   // Menu dédié à la manipulation des notes
    QMenu* _menuOptions;   // Menu dédié aux options de l'application
    QMenu* _menuRelations; // Menu dédié à la gestion des relations

    /* Actions */
    QAction* _actionNvlNote;    // Créer une nouvelle note
    QAction* _actionOpenNote;   // Ouvrir une note existante
    QAction* _actionSaveNote;   // Sauvegarder les modifications sur une note
    QAction* _actionCloseNote;  // Fermer la note actuellement affichée
    QAction* _actionQuitter;    // Quitter l'application
    QAction* _actionDeleteNote; // Supprimer une note
    QAction* _precedentNote;    // Obtenir la version précédente
    QAction* _suivantNote;      // Obtenir la version suivante
    QAction* _manageRelations;  // Gestion des relations

    /* Paramètres */
    QAction* _viderCorbeilleAuto; // Vider la corbeille automatiquement en sortie d'application

    /* Toolbar */
    QToolBar* _mainTool; // Barre d'outils de l'application

    /* Dock gauche - listes de notes */
    QDockWidget* _dockActiveNotes;   // Dock gauche : notes actives dans NotesManager
    QListWidget* _listActiveNotes;   // Liste des notes actives
    QDockWidget* _dockTaches;        // Dock gauche : taches actives dans NotesManager
    QListWidget* _listActiveTasks;   // Liste des taches avec prio ou deadline
    QDockWidget* _dockArchivesNotes; // Dock gauche : notes archivées dans NotesManager
    QListWidget* _listArchivesNotes; // Liste des notes archivées

    /* Dock droit - arborescence */

    /* Handler - Assure qu'il existe une unique MainWindow dans l'application */
    struct Handler {
            MainWindow* _instance; // pointeur sur l'unique instance de la fenêtre principale
            Handler() : _instance(nullptr) {}
            ~Handler() { delete _instance; }
    };
    static Handler _handler;

    /* Constructeur */
    MainWindow();

public:

    /* Méthodes liées au Singleton */

    /**
     * \brief Renvoie une référence sur la fenêtre principale
     *
     * La fonction renvoie une référence sur l'instance de la fenêtre principale (qui est unique). Si elle n'existe pas,
     * elle est crée.
     *
     * \return Référence sur MainWindow
     *
     */

    static MainWindow& getMainWindow();

    /**
     * \brief Libère la mémoire occupée par l'instance principale de l'application
     *
     * La fonction s'occupe de libérer la mémoire occupée par la fenêtre principale.
     *
     */

    static void freeMainWindow();

    /* Méthodes liées à la zone centrale (l'endroit où s'affiche de la note) */

    /**
     * \brief Récupère l'adresse de la zone centrale
     *
     * La fonction récupère et retourne l'adresse de la zone centrale, où sont affichées les notes.
     *
     * \return Pointeur sur CentralZone
     *
     */

    CentralZone* getCentralZone() { return _mainZone; }

    /*  Méthodes liées aux docks de l'application */

    /**
     * \brief Récupère l'id de la note sélectionnée dans le dock des notes actives
     *
     * La fonction récupère l'id de la note actuellement sélectionnée dans le dock des notes actives.
     *
     * \return QString contenant l'id de la note
     *
     */

    QString getSelectedItemFromActiveNotes();

    /**
     * \brief Ajoute une note à la liste des notes
     *
     * La fonction rajoute un élément correspondant à l'id d'une note dans le dock des notes actives.
     *
     * \param id : id de la note à ajouter à la liste
     *
     */

    void addItemToListOfNotes(const QString& id);

    /**
     * \brief Récupère l'id de la note sélectionnée dans le dock des taches
     *
     * La fonction récupère l'id de la note actuellement sélectionnée dans le dock des taches actives.
     *
     * \return QString contenant l'id de la note
     *
     */

    QString getSelectedItemFromActiveTasks();

    /**
     * \brief Ajoute une note à la liste des taches
     *
     * La fonction rajoute un élément correspondant à l'id d'une note dans le dock des taches.
     *
     * \param id : id de la note à ajouter à la liste
     *
     */

    void addItemToListOfTasks(const QString& id);

    /**
     * \brief Récupère l'id de la note sélectionnée dans le dock des notes archivées
     *
     * La fonction récupère l'id de la note actuellement sélectionnée dans le dock des notes archivées.
     *
     * \return QString contenant l'id de la note
     *
     */

    QString getSelectedItemFromArchivedNotes();

    /**
     * \brief Ajoute une note à la liste des notes archivées
     *
     * La fonction rajoute un élément correspondant à l'id d'une note dans le dock des notes archivées.
     *
     * \param id : id de la note à ajouter à la liste
     *
     */

    void addItemToArchives(const QString& id);

    /* Méthodes liées à la sortie de l'application */

    /**
     * \brief Exécute du code à la fermeture de l'application
     *
     * La fonction intercepte les events de fermeture de la MainWindow pour exécuter du code juste avant.
     * C'est la redéfinition d'une méthode de QMainWindow.
     *
     * \param event : closeEvent() généré par l'application
     *
     */

    void closeEvent(QCloseEvent *event);

    /**
     * \brief Sauvegarde les paramètres de l'application
     *
     * La fonction enregistre les paramètres actuels de l'application dans un fichier de configuration.
     * Ce fichier application.conf se trouve à la racine du projet.
     *
     */

    void saveOptions();

    /**
     * \brief Charge les paramètres de l'application
     *
     * La fonction récupère les paramètres de la dernière exécution de l'application pour les restaurer.
     *
     */

    void loadOptions();

public slots:

    /**
     * \brief Ouvre la fenêtre de création de note
     *
     * La fonction génère et affiche l'interface de création d'une note.
     *
     */

    void callCreateNoteWindow();

    /**
     * \brief Ouvre la fenêtre d'ouverture de note
     *
     * La fonction génère et affiche l'interface d'ouverture de note.
     *
     */

    void callOpenNoteWindow();

    /**
     * \brief Ouvre la fenêtre de gestion des relations
     *
     * La fonction génère et affiche l'interface de gestion des relations entre les notes.
     *
     */

    void callRelationsWindow();

    /**
     * \brief Ouvre une note, connaissant son id
     *
     * La fonction affiche la note dont l'id correspond à celui passé en paramètre.
     *
     * \param id : id de la note à afficher
     *
     */

    void openNote(QString id);

    /**
     * \brief Ouvre la note de la liste des notes actives
     *
     * La fonction ouvre la note actuellement sélectionnée dans la liste des notes actives.
     *
     */

    void openNoteFromActiveList();

    /**
     * \brief Ouvre la note de la liste des taches actives
     *
     * La fonction ouvre la note actuellement sélectionnée dans la liste des taches actives.
     *
     */

    void openNoteFromTasks();

    /**
     * \brief Ouvre la note de la liste des notes archivées
     *
     * La fonction ouvre la note actuellement sélectionnée dans la liste des notes archivées.
     *
     */

    void openNoteFromArchives();

    /**
     * \brief Ouvre la version précédente
     *
     * La fonction ouvre la version précédente de la note.
     *
     */

    void openPreviousNote();

    /**
     * \brief Ouvre la version suivante
     *
     * La fonction ouvre la version suivante de la note.
     *
     */

    void openNextNote();

    /**
     * \brief Sauvegarde la note
     *
     * La fonction effectue la sauvegarde de la note actuellement affichée.
     *
     */

    void saveNote();

    /**
     * \brief Autorise la sauvegarde d'une note
     *
     * La fonction permet d'activer l'action de sauvegarde, permettant à l'utilisateur de sauvegarder la note affichée.
     *
     */

    void enableSave() { _actionSaveNote->setDisabled(false); }

    /**
     * \brief Interdit la sauvegarde d'une note
     *
     * La fonction permet de désactiver l'action de sauvegarde, interdisant à l'utilisateur de sauvegarder la note affichée.
     *
     */

    void disableSave() { _actionSaveNote->setDisabled(true); }

    /**
     * \brief Autorise la fermeture d'une note
     *
     * La fonction permet d'activer l'action de fermeture, permettant à l'utilisateur de fermer la note affichée.
     *
     */

    void enableClose() { _actionCloseNote->setDisabled(false); }

    /**
     * \brief Interdit la fermeture d'une note
     *
     * La fonction permet de désactiver l'action de fermeture, interdisant à l'utilisateur de fermer la note affichée.
     *
     */

    void disableClose() { _actionCloseNote->setDisabled(true); }

    /**
     * \brief Autorise la suppression d'une note
     *
     * La fonction permet d'activer l'action de suppression, permettant à l'utilisateur de supprimer la note affichée.
     *
     */

    void enableDelete() { _actionDeleteNote->setDisabled(false); }

    /**
     * \brief Interdit la suppression d'une note
     *
     * La fonction permet d'activer l'action de suppression, interdisant à l'utilisateur de supprimer la note affichée.
     *
     */

    void disableDelete() { _actionDeleteNote->setDisabled(true); }

    /**
     * \brief Ferme la note affichée
     *
     * La fonction interrompt l'affichage de la note actuellement chargée.
     *
     */

    void closeNote();

    /**
     * \brief Supprime la note affichée
     *
     * La fonction permet de supprimer la note actuellement affichée.
     *
     */

    void deleteNote();
};

#endif // MAINWINDOW_H
