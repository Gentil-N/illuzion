#include "illuzion.hpp"

#include <iostream>

namespace ilzn
{
       namespace effect
       {
              void grey_scale(const Picture &picture, Picture &result)
              {
                     for (size_t i = 0; i < picture.height() && i < result.height(); ++i)
                     {
                            for (size_t j = 0; j < picture.width() && j < result.width(); ++j)
                            {
                                   size_t curr_pixel = (i * picture.width() + j) * 4;
                                   byte grey = (picture[curr_pixel] + picture[curr_pixel + 1] + picture[curr_pixel + 2]) / 3;
                                   result[curr_pixel] = grey;
                                   result[curr_pixel + 1] = grey;
                                   result[curr_pixel + 2] = grey;
                                   result[curr_pixel + 3] = picture[curr_pixel + 3];
                            }
                     }
              }

              void invert(const Picture &picture, Picture &result)
              {
                     for (size_t i = 0; i < picture.height() && i < result.height(); ++i)
                     {
                            for (size_t j = 0; j < picture.width() && j < result.width(); ++j)
                            {
                                   size_t curr_pixel = (i * picture.width() + j) * 4;
                                   result[curr_pixel] = 0xff - picture[curr_pixel];
                                   result[curr_pixel + 1] = 0xff - picture[curr_pixel + 1];
                                   result[curr_pixel + 2] = 0xff - picture[curr_pixel + 2];
                                   result[curr_pixel + 3] = picture[curr_pixel + 3];
                            }
                     }
              }

              void flip_horizontal(const Picture &picture, Picture &result)
              {
                     for (size_t i = 0; i < picture.height() && i < result.height(); ++i)
                     {
                            for (size_t j = 0; j < picture.width() && j < result.width(); ++j)
                            {
                                   size_t curr_pixel = (i * picture.width() + j) * 4;
                                   size_t curr_flip_pixel = ((i + 1) * picture.width() - j) * 4;
                                   result[curr_pixel] = picture[curr_flip_pixel];
                                   result[curr_pixel + 1] = picture[curr_flip_pixel + 1];
                                   result[curr_pixel + 2] = picture[curr_flip_pixel + 2];
                                   result[curr_pixel + 3] = picture[curr_flip_pixel + 3];
                            }
                     }
              }

              void resize(const Picture &picture, Picture &result)
              {
                     float coeff_width = (float)picture.width() / (float)result.width();
                     float coeff_height = (float)picture.height() / (float)result.height();
                     //size_t picture_size_limit = picture.width() * picture.height() * 4;
                     for (size_t i = 0; i < result.height(); ++i)
                     {
                            for (size_t j = 0; j < result.width(); ++j)
                            {
                                   size_t curr_pixel = (i * result.width() + j) * 4;
                                   float min_width = j * coeff_width, max_width = (j + 1) * coeff_width;
                                   float min_height = i * coeff_height, max_height = (i + 1) * coeff_height;
                                   size_t first_pixel_width = (size_t)min_width, last_pixel_width = ((size_t)max_width == max_width ? (size_t)max_width - 1 : (size_t)max_width);
                                   size_t first_pixel_height = (size_t)min_height, last_pixel_height = ((size_t)max_height == max_height ? (size_t)max_height - 1 : (size_t)max_height);
                                   size_t average_r = 0, average_g = 0, average_b = 0, average_a = 0;
                                   float weight_tot = 0;
                                   for (size_t k = first_pixel_height; k <= last_pixel_height; ++k)
                                   {
                                          for (size_t l = first_pixel_width; l <= last_pixel_width; ++l)
                                          {
                                                 size_t curr_base_pixel = (k * picture.width() + l) * 4;
                                                 float base_weight_width = 1.0f, base_weight_height = 1.0f;
                                                 if (k == first_pixel_height && k != last_pixel_height)
                                                 {
                                                        base_weight_height = 1.0f - (min_height - first_pixel_height);
                                                 }
                                                 else if (k == last_pixel_height)
                                                 {
                                                        base_weight_height = max_height - last_pixel_height;
                                                 }
                                                 if (l == first_pixel_width && l != last_pixel_width)
                                                 {
                                                        base_weight_width = 1.0f - (min_width - first_pixel_width);
                                                 }
                                                 else if (l == last_pixel_width)
                                                 {
                                                        base_weight_width = max_width - last_pixel_width;
                                                 }
                                                 float base_weight = (base_weight_width * base_weight_height);
                                                 average_r += picture[curr_base_pixel] * base_weight;
                                                 average_g += picture[curr_base_pixel + 1] * base_weight;
                                                 average_b += picture[curr_base_pixel + 2] * base_weight;
                                                 average_a += picture[curr_base_pixel + 3] * base_weight;
                                                 weight_tot += base_weight;
                                          }
                                   }
                                   result[curr_pixel] = average_r / weight_tot;
                                   result[curr_pixel + 1] = average_g / weight_tot;
                                   result[curr_pixel + 2] = average_b / weight_tot;
                                   result[curr_pixel + 3] = average_a / weight_tot;
                            }
                     }
              }
       }
};