#ifndef G_PLUGIN_TREE_HPP
#define G_PLUGIN_TREE_HPP

#include "Live0_Panel.hpp"
#include <de/audio/plugin/GetPluginInfoList.hpp>
#include <QTreeWidget>

#ifndef USE_PLUGIN_DRAG_N_DROP
#define USE_PLUGIN_DRAG_N_DROP
#endif

#ifdef USE_PLUGIN_DRAG_N_DROP
   #include <QMimeData>
   #include <QDrag>
   #include <QDropEvent>
#endif

// ============================================================================
class PluginTree : public QTreeWidget
// ============================================================================
{
   DE_CREATE_LOGGER("PluginTree")
   Q_OBJECT
public:
   PluginTree( App & app, QWidget* parent = 0 );
   ~PluginTree();
signals:
public slots:
protected slots:
protected:
   //void resizeEvent( QResizeEvent* event ) override;
   //void paintEvent( QPaintEvent* event ) override;
   void mousePressEvent( QMouseEvent* event ) override;
   void mouseReleaseEvent( QMouseEvent* event ) override;
   void mouseMoveEvent( QMouseEvent* event ) override;
   void wheelEvent( QWheelEvent* event );
protected:
   void on_itemActivated( QTreeWidgetItem*, int );
protected:
   void populateFromPluginDatabase();

   App & m_app;
   QPixmap m_pixmapVST;
   QIcon m_icoVST;
   QTreeWidgetItem* m_selectedItem;
   QTreeWidgetItem* m_synthNode;
   QTreeWidgetItem* m_effectNode;
   bool m_isDragging;
   int m_dragStartX;
   int m_dragStartY;
};

#endif // G_LOPASS1_HPP
