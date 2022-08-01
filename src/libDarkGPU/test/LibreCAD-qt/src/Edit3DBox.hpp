#ifndef DE_GPU_QWIDGET_EDIT_3DBOX_HPP
#define DE_GPU_QWIDGET_EDIT_3DBOX_HPP

#include <DarkGPU.hpp>
#include <QWidget>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QCheckBox>
#include <V3Edit.hpp>
#include <V4ColorEdit.hpp>

struct Edit3DBox : public QWidget
{
   Q_OBJECT
public:
   Edit3DBox( QWidget* parent );
   ~Edit3DBox() override;
signals:
   void meshCreated( de::gpu::SMesh::SharedPtr );
public slots:
   //void setSceneManager( de::gpu::ISceneManager* smgr );
private slots:
   void onBtnCreate( bool clicked );
   void onBtnHide( bool clicked );

protected:
   void closeEvent( QCloseEvent* event ) override;

   DE_CREATE_LOGGER("Edit3DBox")
   QLabel* m_Label;
   V3Edit* m_Size;
   V4ColorEdit* m_Color;

   QPushButton* m_BtnCreate;
   QPushButton* m_BtnHide;
   bool m_IsHovered = false;
   bool m_IsFocused = false;
   de::gpu::ISceneNode* m_SceneNode;
};

#endif
