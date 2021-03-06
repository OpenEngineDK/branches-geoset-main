// Rendering state node.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Scene/RenderStateNode.h>

namespace OpenEngine {
namespace Scene {

/**
 * Default constructor.
 * No options are set and are thus inherieted from ancestoring
 * RenderStateNodes.
 */
RenderStateNode::RenderStateNode()
    : ISceneNode()
    , enabled(NONE)
    , disabled(NONE)
{

}

RenderStateNode::RenderStateNode(const RenderStateNode& node)
    : ISceneNode(node)
    , enabled(node.enabled)
    , disabled(node.disabled)
{

}

/**
 * Destructor
 */
RenderStateNode::~RenderStateNode() {

}

/**
 * Check if an option is enabled.
 *
 * @param o Frame option(s) to check
 * @return True if enabled
 */
bool RenderStateNode::IsOptionEnabled(RenderStateOption o) const {
    return (o & GetEnabled()) == o;
}

/**
 * Check if an option is disabled.
 *
 * @param o Frame option(s) to check
 * @return True if disabled
 */
bool RenderStateNode::IsOptionDisabled(RenderStateOption o) const {
    return (o & GetDisabled()) == o;
}

/**
 * Get current enabled options set in this RenderStateNode.
 *
 * @return RenderStateOption
 */
RenderStateNode::RenderStateOption RenderStateNode::GetEnabled() const {
    return enabled;
}

/**
 * Get current disabled options set in this RenderStateNode.
 *
 * @return RenderStateOption
 */
RenderStateNode::RenderStateOption RenderStateNode::GetDisabled() const {
    return disabled;
}

/**
 * Add options flag to the already existing,
 * options specified are or'ed with whose already
 * set.
 *
 * @param options option(s) to add.
 */
void RenderStateNode::EnableOption(RenderStateOption options) {
    // removed from disabled
    unsigned int optDis = disabled & ~((unsigned int)options);
    disabled = (RenderStateOption)optDis;

    // add to enabled
    unsigned int optEn = enabled | (unsigned int)options;
    enabled = (RenderStateOption)optEn;
}

/**
 * Add options flag to the already existing,
 * options specified are or'ed with whose already
 * set.
 *
 * @param options option(s) to add.
 */
void RenderStateNode::DisableOption(RenderStateOption options) {
    // removed from enabled
    unsigned int optEn = enabled & ~((unsigned int)options);
    enabled = (RenderStateOption)optEn;

    // add to disabled
    unsigned int optDis = disabled | (unsigned int)options;
    disabled = (RenderStateOption)optDis;
}

/**
 * Toggle specified option.
 *
 * @param options Options to turn on/off.
 */
void RenderStateNode::ToggleOption(RenderStateOption options) {
    if (IsOptionEnabled(options))
        DisableOption(options);
    else if (IsOptionDisabled(options))
        EnableOption(options);
}

RenderStateNode* RenderStateNode::GetInverse() {
    RenderStateNode* inverse = new RenderStateNode();
    RenderStateOption inverseDisabled = inverse->enabled;
    inverse->enabled = inverse->disabled;
    inverse->disabled = inverseDisabled;
    return inverse;
}

const std::string RenderStateNode::ToString() const {
    std::string str(GetClassName());
    if (enabled != NONE) {
        str += "\nEnabled:";
        if ( IsOptionEnabled(TEXTURE) )
            str += " TEXTURE,";
        if ( IsOptionEnabled(SHADER) )
            str += " SHADER,";
        if ( IsOptionEnabled(BACKFACE) )
            str += " BACKFACE,";
        if ( IsOptionEnabled(LIGHTING) )
            str += " LIGHTING,";
        if ( IsOptionEnabled(DEPTH_TEST) )
            str += " DEPTH_TEST,";
        if ( IsOptionEnabled(WIREFRAME) )
            str += " WIREFRAME,";
        if ( IsOptionEnabled(SOFT_NORMAL) )
            str += " SOFT_NORMAL,";
        if ( IsOptionEnabled(HARD_NORMAL) )
            str += " HARD_NORMAL,";
        if ( IsOptionEnabled(BINORMAL) )
            str += " BINORMAL,";
        if ( IsOptionEnabled(TANGENT) )
            str += " TANGENT,";
        str[str.length() - 1] = ' ';
    }
    if (disabled != NONE) {
        str += "\nDisabled:";
        if ( IsOptionDisabled(TEXTURE) )
            str += " TEXTURE,";
        if ( IsOptionDisabled(SHADER) )
            str += " SHADER,";
        if ( IsOptionDisabled(BACKFACE) )
            str += " BACKFACE,";
        if ( IsOptionDisabled(LIGHTING) )
            str += " LIGHTING,";
        if ( IsOptionDisabled(DEPTH_TEST) )
            str += " DEPTH_TEST,";
        if ( IsOptionDisabled(WIREFRAME) )
            str += " WIREFRAME,";
        if ( IsOptionDisabled(SOFT_NORMAL) )
            str += " SOFT_NORMAL,";
        if ( IsOptionDisabled(HARD_NORMAL) )
            str += " HARD_NORMAL,";
        if ( IsOptionDisabled(BINORMAL) )
            str += " BINORMAL,";
        if ( IsOptionDisabled(TANGENT) )
            str += " TANGENT,";
        str[str.length()-1] = ' ';
    }
    return str;
}

} //NS Scene
} //NS OpenEngine
