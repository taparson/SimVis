#ifndef OBJ_H
#define OBJ_H

#include <iostream>
#include <vector>
#include <vrg3d/VRG3D.h>
#include <GL/glut.h>

struct Vertex;

// A simple parser that reads and writes Wavefront .obj files

using namespace G3D;
class OBJ
{
public:
    struct Index
    {
        int vertex;
        int coord;
        int normal;

        Index() : vertex(-1), coord(-1), normal(-1) {}
        Index(int vertex, int coord, int normal) : vertex(vertex), coord(coord), normal(normal) {}
    };

    struct Triangle
    {
        Vertex *v;
        bool visited;

        Index a, b, c;

        Triangle() {}
        Triangle(const Index &a, const Index &b, const Index &c) : a(a), b(b), c(c) {}
    };

    std::vector<Vector3> vertices;
    std::vector<Vector2> coords;
    std::vector<Vector3> normals;
    std::vector<Triangle> triangles;

    void draw() const;
    bool read(const std::string &path, const std::string &texturePath);
    // bool write(const QString &path) const;

private:
    Index getIndex(const std::string &str) const;
    void drawIndex(const Index &index) const;
    static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
    static std::vector<std::string> split(const std::string &s, char delim);
    unsigned int t_id;
};

#endif // OBJ_H
