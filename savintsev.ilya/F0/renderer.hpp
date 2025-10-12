#ifndef RENDERER_HPP
#define RENDERER_HPP
#include <random>
#include <algorithm>
#include <functional>
#include <boost/gil.hpp>
#include <boost/gil/extension/io/bmp.hpp>
#include <shape-utils.hpp>
#include "project-body.hpp"

namespace gil = boost::gil;

namespace savintsev
{
  class Renderer
  {
  public:
    void render_project(const savintsev::Project & proj, const std::string & name, int w, int h)
    {
      gil::rgb8_image_t image(w, h);
      auto view = gil::view(image);

      gil::fill_pixels(view, gil::rgb8_pixel_t(255, 255, 255));
      struct RenderWrapper
      {
        RenderWrapper(Renderer * r, gil::rgb8_view_t & v):
          renderer(r),
          view(v)
        {}
        void operator()(const savintsev::Layer & layer)
        {
          renderer->render_shape(view, layer.second);
        }
        Renderer * renderer;
        gil::rgb8_view_t & view;
      };

      std::for_each(proj.begin(), proj.end(), RenderWrapper(this, view));

      gil::write_view(name + ".bmp", view, gil::bmp_tag{});
    }
  private:
    private:
    void render_shape(gil::rgb8_view_t & view, const savintsev::Shape * shape)
    {
      std::mt19937 rng(std::random_device{}());
      std::uniform_int_distribution<int> dist(50, 240);

      gil::rgb8_pixel_t color
      (
        static_cast< uint8_t >(dist(rng)),
        static_cast< uint8_t >(dist(rng)),
        static_cast< uint8_t >(dist(rng))
      );

      savintsev::point_t points[4];
      size_t point_count = shape->get_all_points(points);

      if (point_count == 2)
      {
        savintsev::point_t rect_points[4];
        rect_points[0] = {points[0].x, points[0].y};
        rect_points[1] = {points[1].x, points[0].y};
        rect_points[2] = {points[1].x, points[1].y};
        rect_points[3] = {points[0].x, points[1].y};
        fill_shape(view, rect_points, 4, color);
      }
      else
      {
        fill_shape(view, points, point_count, color);
      }
    }
    void fill_shape(gil::rgb8_view_t & view, const savintsev::point_t * points, size_t count, gil::rgb8_pixel_t c)
    {
      if (count < 3)
      {
        return;
      }

      int width = view.width();
      int height = view.height();

      for (int y = 0; y < height; ++y)
      {
        for (int x = 0; x < width; ++x)
        {
          double fx = x - width / 2.0;
          double fy = height / 2.0 - y;

          if (is_point_in_polygon(points, count, fx + 0.5, fy + 0.5))
          {
            view(x, y) = c;
          }
        }
      }
    }

    bool is_point_in_polygon(const savintsev::point_t * points, size_t point_count, double x, double y)
    {
      bool inside = false;
      for (size_t i = 0, j = point_count - 1; i < point_count; j = i++)
      {
        double xi = points[i].x, yi = points[i].y;
        double xj = points[j].x, yj = points[j].y;

        bool intersect = ((yi > y) != (yj > y)) && (x < (xj - xi) * (y - yi) / (yj - yi + 1e-15) + xi);
        if (intersect)
        {
          inside = !inside;
        }
      }
      return inside;
    }
  };
}

#endif
