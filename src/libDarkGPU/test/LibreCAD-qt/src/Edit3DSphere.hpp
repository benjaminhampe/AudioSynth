#ifndef DE_GPU_QWIDGET_EDIT_3DSPHERE_HPP
#define DE_GPU_QWIDGET_EDIT_3DSPHERE_HPP

#include <DarkGPU.hpp>
#include <QWidget>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QCheckBox>
#include <V3Edit.hpp>
#include <V4ColorEdit.hpp>

struct Edit3DSphere : public QWidget
{
   Q_OBJECT
public:
   Edit3DSphere( QWidget* parent );
   ~Edit3DSphere() override;
signals:
   void meshCreated( de::gpu::SMesh::SharedPtr );
public slots:
   //void setSceneManager( de::gpu::ISceneManager* smgr );
private slots:
   void onBtnCreate( bool clicked );
   void onBtnHide( bool clicked );

protected:
   void closeEvent( QCloseEvent* event ) override;

   DE_CREATE_LOGGER("Edit3DSphere")
   QLabel* m_Label;
   V3Edit* m_Size;
   QSpinBox* m_TessEast;
   QSpinBox* m_TessNorth;
   V4ColorEdit* m_Color;

   QPushButton* m_BtnCreate;
   QPushButton* m_BtnHide;
   bool m_IsHovered = false;
   bool m_IsFocused = false;
   de::gpu::ISceneNode* m_SceneNode;
};

#endif
