// Geometry node.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Scene/GeometryNode.h>
#include <Math/Vector.h>
#include <Math/Quaternion.h>
#include <Utils/Convert.h>

namespace OpenEngine {
namespace Scene {

using OpenEngine::Geometry::FaceSet;
using OpenEngine::Math::Vector;
using OpenEngine::Math::Quaternion;

/**
 * Default constructor.
 * Creates an initial empty face set.
 */
GeometryNode::GeometryNode() {
    faces = new FaceSet();
}

/**
 * Copy constructor.
 * Copy will contain a new face set (with the same faces).
 *
 * @param node Geometry node to copy.
 */
GeometryNode::GeometryNode(const GeometryNode& node)
    : ISceneNode(node)
{
    faces = new FaceSet(*node.faces);
}
    
/**
 * Face set constructor.
 * The face set will be deleted if replaced by SetFaceSet or upon
 * destruction of the geometry node.
 *
 * @param faces Content of this Geometry Node.
 */
GeometryNode::GeometryNode(FaceSet* faces)
    : faces(faces) {
    
}

/**
 * Destructor.
 * Deletes the contained face set.
 */    
GeometryNode::~GeometryNode() {
    delete faces;
}

/**
 * Get faces this Geometry Node contains.
 * The geometry node retains ownership of the face set.
 *
 * @return FaceSet pointer.
 */
FaceSet* GeometryNode::GetFaceSet() {
    return faces;
}

/**
 * Set FaceSet for this geometry node.
 * This will delete the current face set and bind the new one to
 * the node.
 *
 * @param faces FaceSet pointer.
 */
void GeometryNode::SetFaceSet(FaceSet* faces){
    delete this->faces;
    this->faces = faces;
}

const std::string GeometryNode::ToString() const {
    return GetClassName()
        + "\nFaces: "
        + Utils::Convert::ToString(faces->Size());
}

} //NS Scene
} //NS OpenEngine
