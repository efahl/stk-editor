#ifndef IROAD_HPP
#define IROAD_HPP

#include "spline/ispline.hpp"
#include "mesh/mesh_structs.hpp"

class IRoad :public ISceneNode
{
protected:
    bool            m_valid;
    ISpline*        m_spline;

    f32             m_detail;
    f32             m_width;

    bool            m_closed;
    bool            m_swap_uv;

    aabbox3d<f32>   m_bounding_box;
    virtual void    textureExport(FILE* fp)   = 0;
    virtual void    crossRoadExport(FILE* fp) {};

public:
    IRoad(ISceneNode* parent, ISceneManager* mgr, s32 id, ISpline* s, stringw n);
    IRoad(ISceneNode* parent, ISceneManager* mgr, s32 id, FILE* fp);

    virtual void    refresh() = 0;
    void            clear();

    virtual stringc getTexName() { return ""; }

    void    setClosed(bool b)     { m_closed = b;  refresh();                     }
    void    setSwap(bool b)       { m_swap_uv = b; refresh();                     }
    void    setDetail(float d)    { m_detail = d;  refresh(); setWireFrame(true); }
    void    setWidth(float d)     { m_width = d;   refresh(); setWireFrame(true); }

    bool      isClosedRoad()      { return m_closed; }
    bool      isSwapOn()          { return m_swap_uv;}
    bool      isValid()           { return m_valid;  }
    ISpline*  getSpline()         { return m_spline; }

    virtual void    save(FILE* fp);
    virtual void    setWireFrame(bool b)   {};
    virtual void    setUWrapCount(u32 twc) {};
    virtual void    setVWrapCount(u32 twc) {};
    virtual void    setUOffset(f32 d)      {};
    virtual void    setVOffset(f32 d)      {};

// ----------------------------------------------------------------------------
    // inherited functions - ISceneNode
    void                 OnRegisterSceneNode();
    virtual void         render() = 0;

    virtual const aabbox3d<f32>& getBoundingBox()   const { return m_bounding_box; }
    u32                          getMaterialCount() const { return 1; }

};

#endif
