// 
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_GEOMETRY_SET_H_
#define _OE_GEOMETRY_SET_H_

// @todo: IndexOutOfBounds should be refactored to Core/Exceptions?
// and maybe we should create a OverflowException for the case of
// incrementing to far in an iterator.
#include <Math/Exceptions.h>
#include <Math/Vector.h>

namespace OpenEngine {
namespace Geometry {

/** 
 * Geometry container.
 *
 * A geometry set provides an abstraction over a collection of
 * homogeneous geometrical shapes, such as points, lines and
 * triangles; residing in some dimension, be it the plane (2d) or
 * space (3d). The type parameters \a Dimension and \a Shape.
 *
 * Internally a geometry set contains several separate, but
 * contiguous, memory blocks with geometry information for things such
 * as vertex coordinates, normal vectors and color information. Each
 * block of memory is directly accessible through the GetXxxxArray
 * methods.
 * 
 * Easy access to the individual elements of the collection is
 * provided by an \a Iterator. Access to the data components of an
 * element are automatically adjusted by the iterator. For example:
 * @code
 * // Create a set of triangles in the plane
 * GeometrySet<2, 3> set(2);
 * GeometrySet<2, 3>::Iterator elm;
 * for (elm = set.GetIterator(); elm.HasMore(); elm.Next()) {
 *   elm->vert[0][0] = 1  // First index is a point in the shape 
 *   elm->vert[0][1] = 1  // (0,1,2 for a triangle).
 *   elm->vert[1][0] = 3  // Second is for components of the location
 *   elm->vert[1][1] = 1  // vector, i.e. 0 = x and 1 = y for 2d.
 *   elm->vert[2][0] = 1  // Thus, this code has created a set of two
 *   elm->vert[2][1] = 3  // identical triangles in the plane.
 * }
 * @endcode
 */
template <int Dimension, int Shape>
class GeometrySet {
public:

    // @todo: Arhhgg, I would like to use these two enums for the
    // template parameters, but can't find a way atm. without moving
    // them outside the GeometrySet class.
    enum GeometryDimension {
        D2 = 2,
        D3 = 3
    };
    enum GeometryShape {
        Point    = 1,
        Line     = 2,
        Triangle = 3,
        Quad     = 4
    };

    /**
     * Geometry iterator.
     * Access to data is provided through the pointer (->) operator.
     * Such as @code elm->vert[n][i] @endcode
     *
     * (inherits template parameters)
     */
    class Iterator {

        friend class GeometrySet;
        
    private:
        // Accessors to ensure safety upon indexing
        // T : the type of data elements
        //
        // First level directly guards the data field
        template <class T, int N>
        class Accessor1 {
        public:
            T* p;
            Accessor1() : p(NULL) {}
            Accessor1(T* p) : p(p) {}
            T* GetArray() { return p; }
            Math::Vector<N,T> ToVector() { return Math::Vector<N,T>(p); }
            inline T& operator[](unsigned int i) { 
#if OE_SAFE 
                if (i >= N)
                    throw Math::IndexOutOfBounds(i, 0, N);
#endif
                return *(p + i);
            }
            void operator=(const Math::Vector<N,T>& v){
                p[0] = v.Get(0);
                p[1] = v.Get(1);
                p[2] = v.Get(2);
            }
        };
        // Second level accessor guards a first-level accessor
        template <class T, int N, int M>
        class Accessor2 {
        public:
            T* p;
            Accessor2() : p(NULL) {}
            Accessor2(T* p) : p(p) {}
            inline Accessor1<T, M> operator[](unsigned int i) { 
#if OE_SAFE 
                if (i >= N)
                    throw Math::IndexOutOfBounds(i, 0, N);
#endif
                return Accessor1<T, M>(p + i * M);
            }
        };

        // private constructor (only used by GetIterator)
        Iterator(GeometrySet& set) {
            pos = set.size;
            mem.vert.p = set.vert;
            mem.texc.p = set.texc;
        }

        // current location (backwards, i.e. 0 == end)
        unsigned int pos;

        // inner struct of valid iterator member pointers
        struct Members {
            Accessor2<float, Shape, Dimension> vert;
            Accessor2<float, Shape, 2>         texc;
        } mem;
        
    public:

        /**
         * Empty iterator.
         * Included to allow definitions without initialization
         * expressions.
         * Attempts to access data through an empty iterator will
         * throw an exception (if complied with OE_SAFE).
         */
        Iterator() : pos(0) {
            // accesses to the empty iterator are checked
            // in: operation->()
        }

        /**
         * Access to values in the set via the iterator.
         * The valid members are:
         *   - \a vert : vertex data
         *   - \a norm : soft normals
         *   - \a texc : texture coordinates
         *   - \a colr : colors
         *
         * @throw Core::Exception
         */
        inline Members* operator->() {
#if OE_SAFE
            if (pos == 0)
                throw Core::Exception("Attempt to access an invalid iterator");
#endif
            return &mem;
        }

        /**
         * Check if the iterator has reached the end.
         * Useful for continue conditions in for/while statements
         * ala. STL's: itr != set.end().
         */
        bool HasMore() { return pos > 0; }

        /**
         * Advance the iterator.
         *
         * @throws Core::Exception
         */
        void Next() {
#if OE_SAFE
            if (pos == 0)
                throw Core::Exception("Attempt to advance passed the end of an iterator");
#endif
            --pos;
            mem.vert.p += Dimension * Shape;
        }
    };

    /**
     * Create a geometry set of size 'size'.
     * This will automatically allocate the required memory to store
     * the various geometry components, such as vertex, normal and
     * color data.
     *
     * Will we also initialize the memory to sane defaults? Currently
     * we don't.
     */
    GeometrySet(int size) : size(size) {
        // Allocate (and initialize?) data
        indx = new int[size];
        vert = new float[size * Dimension * Shape];
        texc = new float[size * 2 * Shape]; // always in the plane
    }

    /**
     * Delete routine for a geometry set.
     * Will reclaim the data allocated by the constructor.
     */
    virtual ~GeometrySet() {
        delete[] indx;
        delete[] vert;
        delete[] texc;
    }

    // Get an iterator
    Iterator GetIterator() { return Iterator(*this); }

    // Get the number of geometry elements
    unsigned int GetSize() { return size; }

    // Get the length of an internal array
    unsigned int GetIndxLength() { return size; }
    unsigned int GetVertLength() { return size * Dimension * Shape; }
    //unsigned int GetNormLength() { return size * Dimension * Shape; }
    unsigned int GetTexcLength() { return size * Dimension * Shape; }
    //unsigned int GetColrLength() { return size * Dimension * Shape; }

    // Get the direct pointer of an internal array
    int*   GetIndxArray() { return indx; }
    float* GetVertArray() { return vert; }
    //float* GetNormArray() { return norm; }
    float* GetTexcArray() { return texc; }
    //float* GetColrArray() { return colr; }

private:
    friend class GeometryIterator;
    int    size;
    int*   indx;
    float* vert;
    //float* norm;
    float* texc;
    //float* colr;
};

} // NS Geometry
} // NS OpenEngine

#endif // _OE_GEOMETRY_SET_H_
