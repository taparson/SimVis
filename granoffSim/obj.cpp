#include "obj.h"
// #include <qgl.h>
// #include <QFile>
// #include <QTextStream>

#include <iostream>
#include <fstream>
#include <iostream>
#include "SOIL.h"

using namespace std;

void OBJ::draw(Vector3 translate) const
// {
//     glEnable(GL_TEXTURE_2D);
//     glEnable(GL_DEPTH_TEST);
//     glBindTexture(GL_TEXTURE_2D,t_id);
   
//     glBegin(GL_TRIANGLES);
//     for (int i = 0; i < triangles.size(); i++) {
//         // std::cout << "here!" << std::endl;
//         Triangle tri = triangles[i];
//         drawIndex(tri.a);
//         drawIndex(tri.b);
//         drawIndex(tri.c);
//     }
//     glEnd();
//     glDisable(GL_TEXTURE_2D);
// }

// {
//     // std::cout << "woof" << std::endl;
//     // std::cout << "size: " << triangles.size() << std::endl;
//     // std::cout << "woof" << std::endl;
//     glEnable(GL_TEXTURE_2D);
//     glEnable(GL_DEPTH_TEST);
//     glBindTexture(GL_TEXTURE_2D,t_id);
//     int strideMult = 3;
//     if(tex) strideMult+=2;
//     if(norm) strideMult+=3;

//     unsigned int stride = sizeof(float) * strideMult;
//     glBegin(GL_TRIANGLES);

//     for (int i = 0; i < numVertices; i++) {
//         // std::cout << "here!" << std::endl;
//         if(tex) glTexCoord2f(vbo[i*strideMult + 3],vbo[i*strideMult + 4]);
//         if(norm) glNormal3f(vbo[i*strideMult + 5],vbo[i*strideMult + 6],vbo[i*strideMult + 7]);
//         glVertex3f(vbo[i*strideMult + 0],vbo[i*strideMult + 1],vbo[i*strideMult + 2]);
//     }
//     glEnd();
//     glDisable(GL_TEXTURE_2D);
// }

{
    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_NORMALIZE);
    //glPushMatrix();
    //glTranslatef(-1.0f*center.x,-1.0f*center.y,-1.0f*center.z);
    //glScalef(.001,.001,.001);

    translation += translate;

    if(tex) {glBindTexture(GL_TEXTURE_2D,t_id); glEnable(GL_TEXTURE_2D);glEnableClientState(GL_TEXTURE_COORD_ARRAY);}

    glBindBuffer(GL_ARRAY_BUFFER,v_id);
    glEnableClientState(GL_VERTEX_ARRAY);
    if(norm) glEnableClientState(GL_NORMAL_ARRAY);
    int strideMult = 3;
    if(tex) strideMult+=2;
    if(norm) strideMult+=3;

    int count = 3;
    unsigned int stride = sizeof(float) * strideMult;
    glVertexPointer(3,GL_FLOAT,stride,(void *) 0);
    if(tex){glTexCoordPointer(2,GL_FLOAT,stride,(void *) (count*sizeof(float))); count += 2;}
    if(norm)glNormalPointer(GL_FLOAT,stride,(void *) (count*sizeof(float)));
    glDrawArrays(GL_TRIANGLES,0,numVertices);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    //glPopMatrix();
    glDisable(GL_TEXTURE_2D);

}


bool OBJ::read(const std::string &path, const std::string &texturePath)
{
    // Open the file
    tex = true;
    norm = false;
    std::cout << "reading..." << std::endl;
    if(texturePath != "")  {
        unsigned int id = SOIL_load_OGL_texture(texturePath.c_str(),SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
        if(id == 0) return -1;
        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);

        t_id = id;    
    }
    else tex = false;

    std::string line;
    ifstream myfile (path.c_str(),std::ifstream::in);
    if (myfile.is_open())
    {   
        Vector3 mins = Vector3(std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),std::numeric_limits<float>::max());
        Vector3 maxes = Vector3(std::numeric_limits<float>::min(),std::numeric_limits<float>::min(),std::numeric_limits<float>::min());
        std::vector<float> vbo;
        while ( getline (myfile,line) )
        {
            // cout << line << '\n';
            std::vector<std::string> parts = OBJ::split(line,' ');
            if (parts.size() == 0) continue;
            std::vector<std::string> newParts;
            float part1,part2,part3;
            if(parts.size() >= 4)  {
                istringstream(parts[1]) >> part1;
                istringstream(parts[2]) >> part2;
                istringstream(parts[3]) >> part3;
                // newParts.append(part1);
                // newParts.append(part2);
                // newParts.append(part3);
            }
            else if(parts.size() == 3)  {
                istringstream(parts[1]) >> part1;
                istringstream(parts[2]) >> part2;
                // newParts.append(part1);
                // newParts.append(part2);
                // newParts.append(part3);
            }
            else{}
            if (parts[0] == "v" && parts.size() >= 4) {
                vertices.push_back(Vector3(part1, part2, part3));
                if(part1 > maxes.x) maxes.x = part1;
                if(part2 > maxes.y) maxes.y = part2;
                if(part3 > maxes.z) maxes.z = part3;
                if(part1 < mins.x) mins.x = part1;
                if(part2 < mins.y) mins.y= part2;
                if(part3 < mins.z) mins.z= part3;
            } else if (parts[0] == "vt" && parts.size() >= 3) {
                coords.push_back(Vector2(part1, part2));
            } else if (parts[0] == "vn" && parts.size() >= 4) {
                normals.push_back(Vector3(part1, part2, part3));
            } 
            // else {continue;}
            else if (parts[0] == "f" && parts.size() >= 4) {
                // Convert polygons into triangle fans
                Index a = getIndex(parts[1]);
                Index b = getIndex(parts[2]);
                for (int i = 3; i < parts.size(); i++) {
                    Index c = getIndex(parts[i]);
                    //triangles.push_back(Triangle(a, b, c));
                    std::vector<Index> inds;
                    inds.push_back(a);
                    inds.push_back(b);
                    inds.push_back(c);
                    for(int j = 0; j < 3; j++)  {
                        Index ind = inds[j];
                        if (ind.vertex >= 0 && ind.vertex < vertices.size())  {
                            vbo.push_back(vertices[ind.vertex].x);
                            vbo.push_back(vertices[ind.vertex].y);
                            vbo.push_back(vertices[ind.vertex].z);
                            numVertices += 1;
                        }
                        if(ind.coord >= 0 && ind.coord < coords.size() && tex)  {
                            std::cout << "meow" << std::endl;
                            vbo.push_back(coords[ind.coord].x);
                            vbo.push_back(coords[ind.coord].y);
                        }
                        if (ind.normal >= 0 && ind.normal < normals.size())  {
                            vbo.push_back(normals[ind.normal].x);
                            vbo.push_back(normals[ind.normal].y);
                            vbo.push_back(normals[ind.normal].z);
                        }
                    }
                    b = c;
                }
            }

        }
        myfile.close();
        //if (coords.size() > 0) tex = true;
        if (normals.size() > 0) norm = true;
        //constructVBOs(vbo);
        center = Vector3((maxes.x-mins.x)/2.0f,(maxes.y-mins.y)/2.0f,(maxes.z-mins.z)/2.0f);
        vertices = std::vector<Vector3>();
        normals = std::vector<Vector3>();
        coords = std::vector<Vector2>();
        std::cout << "binding vbo..." << std::endl;
        v_id = bindVBO(vbo);
        std::cout << "done reading file" << std::endl;
    }

  else cout << "Unable to open file"; 

    return true;
}

void OBJ::constructVBOs()  {
    std::vector<float> vbo;
    numVertices = 0;
    std::cout << triangles.size() << std::endl;
    for(int i = 0; i < triangles.size(); i++)  {
        Triangle t = triangles[i];
        std::vector<Index> inds;
        inds.push_back(t.a);
        inds.push_back(t.b);
        inds.push_back(t.c);
        for(int j = 0; j < 3; j++)  {
            Index ind = inds[j];
            if (ind.vertex >= 0 && ind.vertex < vertices.size())  {
                vbo.push_back(vertices[ind.vertex].x);
                vbo.push_back(vertices[ind.vertex].y);
                vbo.push_back(vertices[ind.vertex].z);
                numVertices += 1;
            }
            if(ind.coord >= 0 && ind.coord < coords.size())  {
                vbo.push_back(coords[ind.coord].x);
                vbo.push_back(coords[ind.coord].y);
            }
            if (ind.normal >= 0 && ind.normal < normals.size())  {
                vbo.push_back(normals[ind.normal].x);
                vbo.push_back(normals[ind.normal].y);
                vbo.push_back(normals[ind.normal].z);
            }
        }
    }
    vertices = std::vector<Vector3>();
    normals = std::vector<Vector3>();
    coords = std::vector<Vector2>();
    triangles = std::vector<Triangle>();
    v_id = bindVBO(vbo);

}

unsigned int OBJ::bindVBO(std::vector<float> &vbo)  {
    unsigned int id;
    glGenBuffers(1,&id);
    glBindBuffer(GL_ARRAY_BUFFER,id);

    glBufferData(GL_ARRAY_BUFFER,sizeof(float)*vbo.size(),vbo.data(),GL_STATIC_DRAW);//&(*vbo.begin())
    glBindBuffer(GL_ARRAY_BUFFER,0);
    return id;
}

std::vector<std::string> &OBJ::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> OBJ::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

// static QString str(const Vector2 &v) { return QString("%1 %2").arg(v.x).arg(v.y); }
// static QString str(const Vector3 &v) { return QString("%1 %2 %3").arg(v.x).arg(v.y).arg(v.z); }

// static QString str(const OBJ::Index &i)
// {
//     if (i.normal >= 0) {
//         if (i.coord >= 0) return QString("%1/%2/%3").arg(i.vertex + 1).arg(i.coord + 1).arg(i.normal + 1);
//         return QString("%1//%2").arg(i.vertex + 1).arg(i.normal + 1);
//     } else {
//         if (i.coord >= 0) return QString("%1/%2").arg(i.vertex + 1).arg(i.coord + 1);
//         return QString("%1").arg(i.vertex + 1);
//     }
// }

// bool OBJ::write(const QString &path) const
// {
//     // Open the file
//     QFile file(path);
//     if (!file.open(QFile::WriteOnly | QFile::Text)) return false;
//     QTextStream f(&file);

//     // Write the file
//     foreach (const Vector3 &vertex, vertices) f << "v " << str(vertex) << '\n';
//     foreach (const Vector2 &coord, coords) f << "vt " << str(coord) << '\n';
//     foreach (const Vector3 &normal, normals) f << "vn " << str(normal) << '\n';
//     foreach (const Triangle &tri, triangles) f << "f " << str(tri.a) << ' ' << str(tri.b) << ' ' << str(tri.c) << '\n';

//     return true;
// }

inline int relativeIndex(int index, int count)
{
    return index >= 0 ? index - 1 : count + index;
}

OBJ::Index OBJ::getIndex(const std::string &str) const
{
    // QStringList parts = str.split('/');
    std::vector<std::string> parts = OBJ::split(str,'/');
    int part0,part1,part2;
    if(parts.size() > 0) istringstream(parts[0]) >> part0;
    if(parts.size() > 1) istringstream(parts[1]) >> part1;
    if(parts.size() > 2) istringstream(parts[2]) >> part2;
    int vertex = parts.size() > 0 ? relativeIndex(part0, vertices.size()) : -1;
    int coord = parts.size() > 1 ? relativeIndex(part1, coords.size()) : -1;
    int normal = parts.size() > 2 ? relativeIndex(part2, normals.size()) : -1;
    return Index(vertex, coord, normal);
    // return Index(0,0,0);
}

void OBJ::drawIndex(const Index &index) const
{
    if (index.coord >= 0 && index.coord < coords.size()) glTexCoord2f(coords[index.coord].x,coords[index.coord].y);
    if (index.normal >= 0 && index.normal < normals.size()) glNormal3f(normals[index.normal].x,normals[index.normal].y,normals[index.normal].z);
    if (index.vertex >= 0 && index.vertex < vertices.size()) glVertex3f(vertices[index.vertex].x,vertices[index.vertex].y,vertices[index.vertex].z);
}
