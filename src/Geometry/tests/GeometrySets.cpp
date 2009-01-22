#include <Testing/Testing.h>

#include <Geometry/GeometrySet.h>
#include <Math/Exceptions.h>

using namespace std;
using namespace OpenEngine;
using namespace OpenEngine::Geometry;

int test_main(int argc, char* argv[]) {

    // triangles in the plane
    GeometrySet<2, 3> set(2);
    GeometrySet<2, 3>::Iterator elm;
    float* p = set.GetVertArray();

#if OE_SAFE
    // check that we can not use an uninitialized iterator
    OE_CHECK_THROW(elm->vert[0][0], Core::Exception);
#endif

    elm = set.GetIterator();

    // write two elements and check the result
    elm->vert[0][0] = 1; // 0 - x
    elm->vert[0][1] = 2; // 0 - y
    elm->vert[1][0] = 3; // 1 - x
    elm->vert[1][1] = 4; // 1 - y
    elm->vert[2][0] = 5; // and so on
    elm->vert[2][1] = 6;
    OE_CHECK(*p++ == 1 && *p++ == 2);
    OE_CHECK(*p++ == 3 && *p++ == 4);
    OE_CHECK(*p++ == 5 && *p++ == 6);

    // increment and write another two elements
    elm.Next();
    elm->vert[0][0] = 6;
    elm->vert[0][1] = 5;
    elm->vert[1][0] = 4;
    elm->vert[1][1] = 3;
    elm->vert[2][0] = 2;
    elm->vert[2][1] = 1;
    OE_CHECK(*p++ == 6 && *p++ == 5);
    OE_CHECK(*p++ == 4 && *p++ == 3);
    OE_CHECK(*p++ == 2 && *p++ == 1);

    // check that the first elements are still there
    p = set.GetVertArray();
    OE_CHECK(*p++ == 1 && *p++ == 2);
    OE_CHECK(*p++ == 3 && *p++ == 4);
    OE_CHECK(*p++ == 5 && *p++ == 6);

#if OE_SAFE
    // check out-of-bound exceptions
    OE_CHECK_THROW(elm->vert[3][0], Math::IndexOutOfBounds);
    OE_CHECK_THROW(elm->vert[0][2], Math::IndexOutOfBounds);

    // check iteration overflow invalidates access
    elm.Next();
    OE_CHECK_THROW(elm->vert[0][0], Core::Exception);
    OE_CHECK_THROW(elm.Next(),      Core::Exception);
#endif

    // test iteration
    int i = 0;
    for (elm = set.GetIterator(); elm.HasMore(); elm.Next()) i++;
    OE_CHECK(i == 2);

    // create a set of lines in space
    GeometrySet<3, 2> set2(2);
    GeometrySet<3, 2>::Iterator elm2;
    // test access to texture component
    elm2 = set2.GetIterator();
    elm2->texc[0][0] = 1;
    elm2->texc[0][1] = 2;
    OE_CHECK(*set2.GetTexcArray() == 1 && *(set2.GetTexcArray()+1) == 2);
    // attempt to access the third component (z) of a 2-tuple (x,y)
    // i.e. texture coordinates are in the plane, even for a set in space
    OE_CHECK_THROW(elm2->texc[0][2], Core::Exception);

    return 0;
}
