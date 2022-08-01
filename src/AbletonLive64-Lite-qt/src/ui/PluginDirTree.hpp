#ifndef G_PLUGIN_DIRTREE_HPP
#define G_PLUGIN_DIRTREE_HPP

#include "LiveData.hpp"
#include <QTreeWidget>

struct App;

// ============================================================================
class PluginDirTree : public QTreeWidget
// ============================================================================
{
   DE_CREATE_LOGGER("PluginDirTree")
   Q_OBJECT
public:
   PluginDirTree( App & app, QWidget* parent = 0 );
   ~PluginDirTree() override {}
signals:
public slots:

   void createLiveTreeHeader();
   void updateFromPluginDb();
protected slots:
   void on_itemActivated( QTreeWidgetItem*, int );
protected:

   App & m_app;
   QTreeWidgetItem* m_selected;

};


#endif // G_LOPASS1_HPP
