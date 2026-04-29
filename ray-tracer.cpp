#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "vector3.hpp"
#include "triangle.hpp"
#include <cmath>
#include <vector>
#include <iostream>

using namespace std;
vector<float> green_shader(Vector3 position, Vector3 normal, Vector3 uv, Vector3 light)
{
  Vector3 lightDirection = light;
  float diffuse_intensity = lightDirection.normalized().dot(normal.normalized());
  diffuse_intensity = max(0.f, diffuse_intensity);
  float intensity = .2f + diffuse_intensity;
  float r = 0.f * diffuse_intensity;
  float g = 1.f * diffuse_intensity;
  float b = 0.f * diffuse_intensity;
  return {r, g, b};
}
Vector3 orange_shader(Vector3 position, Vector3 normal, Vector3 uv, Vector3 light)
{
  Vector3 lightDirection = light;
  float diffuse_intensity = lightDirection.normalized().dot(normal.normalized());
  diffuse_intensity = max(0.f, diffuse_intensity);
  float intensity = .2f + diffuse_intensity;
  float r = .8f * diffuse_intensity;
  float g = .5f * diffuse_intensity;
  float b = 0.f * diffuse_intensity;
  return {r, g, b};
}
Vector3 red_shader(Vector3 position, Vector3 normal, Vector3 uv, Vector3 light)
{
  return {1.0f, 0.0f, 0.0f};
  Vector3 lightDirection = light;
  float diffuse_intensity = lightDirection.normalized().dot(normal.normalized());
  diffuse_intensity = max(0.f, diffuse_intensity);
  float intensity = .2f + diffuse_intensity;
  float r = 1.f * diffuse_intensity;
  float g = 0.f * diffuse_intensity;
  float b = 0.f * diffuse_intensity;
  return {r, g, b};
}
Vector3 yellow_shader(Vector3 position, Vector3 normal, Vector3 uv, Vector3 light)
{
  Vector3 lightDirection = light;
  float diffuse_intensity = lightDirection.normalized().dot(normal.normalized());
  diffuse_intensity = max(0.f, diffuse_intensity);
  float intensity = .2f + diffuse_intensity;
  float r = .5f * diffuse_intensity;
  float g = .8f * diffuse_intensity;
  float b = 0.f * diffuse_intensity;
  return {r, g, b};
}
Vector3 purple_shader(Vector3 position, Vector3 normal, Vector3 uv, Vector3 light)
{
  Vector3 lightDirection = light;
  float diffuse_intensity = lightDirection.normalized().dot(normal.normalized());
  diffuse_intensity = max(0.f, diffuse_intensity);
  float intensity = .2f + diffuse_intensity;
  float r = .8f * diffuse_intensity;
  float g = 0.f * diffuse_intensity;
  float b = .8f * diffuse_intensity;
  return {r, g, b};
}
bool trace_ray(vector<Triangle> triangles, Vector3 ray_origin, Vector3 ray_direction, Triangle &closest_triangle, float &closest_hit, Vector3 &triangle_collision, Vector3 &triangle_normal)
{
  for (const auto &triangle : triangles)
  {
    // if(x==100 && y==100){
    //   int a = 0
    // }
    // float _x = x / w * 2 - 1;
    // float _y = -(y / h * 2 - 1);
    // Vector3 target = {_x, _y, -1};
    // Vector3 ray_direction = (Vector3{_x, _y, -1}.sub(camera_origin)).normalized();
    Vector3 p0 = triangle.p0;
    Vector3 p1 = triangle.p1;
    Vector3 p2 = triangle.p2;
    Vector3 leg1 = p1.sub(p0);
    Vector3 leg2 = p2.sub(p0);
    Vector3 normal = leg1.cross(leg2);
    float d = -normal.dot(p0);
    // plane location
    float t = (-d - ray_origin.dot(normal)) / (normal.dot(ray_direction));
    // if (t < closest_hit)
    // {
    //   // closest_hit = t;
    // }
    // else
    // {
    //   continue;
    // }
    Vector3 plane_collision = ray_origin.add(ray_direction.times(t));
    // find whether point is on triangle
    //  Vector3 normal = leg1.cross(leg2);
    float area_triangle = normal.length() / 2;
    Vector3 p_p0 = plane_collision.sub(p0);
    Vector3 p_p1 = plane_collision.sub(p1);
    Vector3 p_p2 = plane_collision.sub(p2);
    float area_opposite_p0 = p_p1.cross(p_p2).length() / 2;
    float area_opposite_p1 = p_p0.cross(p_p2).length() / 2;
    float area_opposite_p2 = p_p0.cross(p_p1).length() / 2;
    float barycentric_p0 = area_opposite_p0 / area_triangle;
    float barycentric_p1 = area_opposite_p1 / area_triangle;
    float barycentric_p2 = area_opposite_p2 / area_triangle;
    float tolerance = .00001;
    bool in_triangle = abs(1 - (barycentric_p0 + barycentric_p1 + barycentric_p2)) < tolerance;
    if (!in_triangle)
      continue;
    closest_hit = t;
    triangle_collision = plane_collision;
    triangle_normal = normal;
    closest_triangle = triangle;
  }
  return closest_hit != INFINITY;
}
int main()
{
  vector<Vector3> lights;
  lights.push_back({1, 2, .5});
  lights.push_back({0, 1, -.5});
  Vector3 camera_origin = {0, 0, 0};
  const float size = 5;
  const float distance = -15;

   vector<Triangle> triangles;
  Vector3 nxnynz = {-size, -size * 1.5f, -size + distance};
  Vector3 nxnypz = {-size, -size * 1.5f, size + distance};
  Vector3 nxpynz = {-size, +size * 1.5f, -size + distance};
  Vector3 nxpypz = {-size, +size * 1.5f, size + distance};
  Vector3 pxnynz = {+size, -size * 1.5f, -size + distance};
  Vector3 pxnypz = {+size, -size * 1.5f, size + distance};
  Vector3 pxpynz = {+size, +size * 1.5f, -size + distance};
  Vector3 pxpypz = {+size, +size * 1.5f, size + distance};

  // Bottom
  triangles.push_back({nxnynz, nxnypz, pxnypz, orange_shader});
  triangles.push_back({nxnynz, pxnypz, pxnynz, orange_shader});

  // Top
  triangles.push_back({nxpynz, pxpypz, nxpypz, orange_shader});
  triangles.push_back({nxpynz, pxpynz, pxpypz, orange_shader});

  // Left
  triangles.push_back({nxnynz, nxpynz, nxnypz, orange_shader});
  triangles.push_back({nxpypz, nxnypz, nxpynz, orange_shader});

  // Right
  triangles.push_back({pxnynz, pxnypz, pxpynz, orange_shader});
  triangles.push_back({pxpypz, pxpynz, pxnypz, orange_shader});

  // Back
  triangles.push_back({nxpynz, nxnynz, pxpynz, orange_shader});
  triangles.push_back({pxnynz, pxpynz, nxnynz, orange_shader});

  Vector3 background_color = {0.1f, 0.1f, .1f};
  const int w = 640;
  const int h = 640;

  unsigned char pixels[w * h * 3];

  for (float y = 0; y < h; y++)
  {
    for (float x = 0; x < w; x++)
    {
      float closest_hit = INFINITY;
      Vector3 closest_pixel{0, 0, 0};

      float _x = x / w * 2 - 1;
      float _y = -(y / h * 2 - 1);
      Vector3 target = {_x, _y, -1};
      Vector3 ray_direction = (Vector3{_x, _y, -1}.sub(camera_origin)).normalized();
      Vector3 plane_collision;
      Vector3 normal;
      Triangle triangle;
      if (trace_ray(triangles, camera_origin, ray_direction, triangle, closest_hit, plane_collision, normal))
      {
        for (const auto &light : lights)
        {
          closest_pixel = closest_pixel.add(triangle.shader(plane_collision, normal, {0, 0, 0}, light));
        }
        // closest_hit = t;
      }

      int idx = (y * w + x) * 3;
      Vector3 final_color = closest_hit != INFINITY ? closest_pixel : background_color;
      pixels[idx] = (unsigned char)(255 * min(1.f, final_color.x));
      pixels[idx + 1] = (unsigned char)(255 * min(1.f, final_color.y));
      pixels[idx + 2] = (unsigned char)(255 * min(1.f, final_color.z));
    }
  }

  stbi_write_png("image.png", w, h, 3, pixels, w * 3);
  return 0;
}
