#include "VertexTableModel.hpp"
#include <QDebug>
#include <QVBoxLayout>
#include <QSplitter>
#include <QHeaderView>

VertexTableModel::VertexTableModel( QObject *parent )
   : QAbstractTableModel( parent )
   , m_MeshBuffer( nullptr )
{

}

VertexTableModel::~VertexTableModel()
{

}

int
VertexTableModel::rowCount( QModelIndex const & parent ) const
{
   if ( !m_MeshBuffer ) return 0;
   return m_MeshBuffer->Vertices.size();
}

int
VertexTableModel::columnCount( QModelIndex const & parent ) const
{
   if ( !m_MeshBuffer ) return 0;
   return 9; // 9 vertex attributes
}

QVariant
VertexTableModel::data( QModelIndex const & index, int role ) const
{
   if ( !m_MeshBuffer ) return 0.0f;
   int x = index.column();
   int y = index.row();

   auto const & vertex = m_MeshBuffer->Vertices[ y ];
        if ( x == 0 ) return vertex.pos.x;
   else if ( x == 1 ) return vertex.pos.y;
   else if ( x == 2 ) return vertex.pos.z;
   else if ( x == 3 ) return vertex.normal.x;
   else if ( x == 4 ) return vertex.normal.y;
   else if ( x == 5 ) return vertex.normal.z;
   else if ( x == 6 ) return vertex.color;
//   else if ( x == 6 ) return de::RGBA_R(vertex.color);
//   else if ( x == 7 ) return de::RGBA_G(vertex.color);
//   else if ( x == 8 ) return de::RGBA_B(vertex.color);
//   else if ( x == 9 ) return de::RGBA_A(vertex.color);
   else if ( x == 7 ) return vertex.tex.x;
   else if ( x == 8 ) return vertex.tex.y;
}
