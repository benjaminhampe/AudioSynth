#ifndef DE_GPU_MESH_TOOL_QWIDGET_HPP
#define DE_GPU_MESH_TOOL_QWIDGET_HPP

#include <Canvas3D.hpp>
#include <QWidget>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <Edit3DBox.hpp>
#include <Edit3DSphere.hpp>
//#include <Edit3DCylinder.hpp>
//#include <Edit3DTube.hpp>

struct MeshTool : public QWidget
{
   Q_OBJECT
public:
   MeshTool( QWidget* parent );
   ~MeshTool() override;
signals:
   void meshCreated( de::gpu::SMesh::SharedPtr ); // emitted to SceneManager

public slots:
   void emit_meshCreated( de::gpu::SMesh::SharedPtr );
   void setCanvas3D( Canvas3D* canvas );     // controls driver
   void setSceneNode(de::gpu::ISceneNode*);  // controls node
   void setMesh(de::gpu::SMesh::SharedPtr);            // controls mesh
   void saveScreenShot();
protected slots:
   void onBtnSelMesh( bool checked );
   void onBtnSelFace( bool checked );
   void onBtnSelVertex( bool checked );
   void onBtnAddPoint( bool checked );
   void onBtnAddLine( bool checked );
   void onBtnAddTriangle( bool checked );
   void onBtnAddQuad( bool checked );
   void onBtnAddHexagon( bool checked );
   void onBtnAddPath( bool checked );
   void onBtnAddCircle( bool checked );
   void onBtnAddRing( bool checked );
   void onBtnAddBox( bool checked );
   void onBtnAddSphere( bool checked );
   void onBtnAddCone( bool checked );
   void onBtnAddCylinder( bool checked );
   void onBtnAddPyramide( bool checked );
   void onBtnAddTube( bool checked );
   void onBtnAddTorus( bool checked );
   void onBtnAddRoundRect( bool checked );
   void onBtnTRS_Move( bool checked );
   void onBtnTRS_Rotate( bool checked );
   void onBtnTRS_Scale( bool checked );
   void onBtnFlipMeshNormals( bool checked );
   void onBtnFlipMeshX( bool checked );
   void onBtnFlipMeshY( bool checked );
   void onBtnFlipMeshZ( bool checked );
   void onBtnFlipMeshXY( bool checked );
   void onBtnFlipMeshYZ( bool checked );
   void onBtnFlipMeshXZ( bool checked );
   void onSetFpsTimer( int time_per_frame_in_ms );
   void updateFpsLabel();
   void updateCanvasInfoLabel();

   void showEditorAddBox3D();
   void ps_Box3D_customContextMenuRequested( QPoint const & pos );
   void ps_Sphere3D_customContextMenuRequested( QPoint const & pos );
protected:
   void timerEvent( QTimerEvent* e ) override;
public:
   DE_CREATE_LOGGER("MeshTool")
   Edit3DBox* m_Edit3DBox;
   Edit3DSphere* m_Edit3DSphere;
//   QPushButton* m_BtnAddFaceVertex; // Add face vertex
//   QPushButton* m_BtnAddFaceLine;   // Add face line
//   QPushButton* m_BtnAddFaceCircle; // Add face quadratic circle/ellipse arc
//   QPushButton* m_BtnAddFaceCatmull;// Add face catmull rom spline arc
//   QPushButton* m_BtnAddFaceBezier; // Add face catmull rom spline arc
   QPushButton* m_BtnSelMesh;
   //QPushButton* m_BtnSelBuffer;
   QPushButton* m_BtnSelFace;
   QPushButton* m_BtnSelVertex;


   QPushButton* m_BtnTranslate;
   QPushButton* m_BtnRotate;
   QPushButton* m_BtnScale;

   QPushButton* m_BtnAddPoint;
   QPushButton* m_BtnAddLine;
   QPushButton* m_BtnAddTriangle;
   QPushButton* m_BtnAddQuad;
   QPushButton* m_BtnAddCircle;
   QPushButton* m_BtnAddRing;
   QPushButton* m_BtnAddRoundRect;
   QPushButton* m_BtnAddHexagon;
   QPushButton* m_BtnAddPath;

   QPushButton* m_BtnAdd3DBox;
   QPushButton* m_BtnAddSphere;
   QPushButton* m_BtnAddCylinder;
   QPushButton* m_BtnAddCone;
   QPushButton* m_BtnAddTube;
   QPushButton* m_BtnAddTorus;

   QPushButton* m_BtnFlipX;   // negate x
   QPushButton* m_BtnFlipY;   // negate y
   QPushButton* m_BtnFlipZ;   // negate z
   QPushButton* m_BtnFlipNormals; // negate normals
   QPushButton* m_BtnFlipXY; // rotate z?
   QPushButton* m_BtnFlipYZ; // rotate x?
   QPushButton* m_BtnFlipXZ; // rotate y?

   Canvas3D* m_Canvas;
   de::gpu::ISceneNode* m_Node;
   de::gpu::SMesh::SharedPtr m_Mesh;

   int m_FpsDisplayUpdateTimerId;
   QLabel*   m_FpsDisplay;
   QSpinBox* m_FrameTimeEdit;
   QPushButton* m_ScreenShotButton;
};

#endif
