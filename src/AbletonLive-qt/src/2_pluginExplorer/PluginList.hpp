#ifndef G_PLUGIN_LIST_HPP
#define G_PLUGIN_LIST_HPP

#include "Live0_Panel.hpp"
#include "PluginTree.hpp"

#ifndef USE_PLUGIN_DRAG_N_DROP
#define USE_PLUGIN_DRAG_N_DROP
#endif

#ifdef USE_PLUGIN_DRAG_N_DROP
   #include <QMimeData>
   #include <QDrag>
   #include <QDropEvent>
#endif

// ============================================================================
class PluginList : public QWidget
// ============================================================================
{
   DE_CREATE_LOGGER("PluginList")
   Q_OBJECT
public:
   PluginList( App & app, QWidget* parent = 0 );
   ~PluginList();
signals:
public slots:
protected slots:
protected:
   //void resizeEvent( QResizeEvent* event ) override;
   void paintEvent( QPaintEvent* event ) override;
   void mousePressEvent( QMouseEvent* event ) override;
   void mouseReleaseEvent( QMouseEvent* event ) override;
   void mouseMoveEvent( QMouseEvent* event ) override;
   void wheelEvent( QWheelEvent* event );
protected:
   void on_itemActivated( QTreeWidgetItem*, int );
protected:
   App & m_app;
   PluginTree* m_pluginTree;
   QTreeWidgetItem* m_selectedItem;
   bool m_isDragging;
   int m_dragStartX;
   int m_dragStartY;
};

#endif // G_LOPASS1_HPP
