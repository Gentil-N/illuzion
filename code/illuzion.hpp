#pragma once
#ifndef __ILLUZION_H__
#define __ILLUZION_H__

#include <stdint.h>
#include <stdlib.h>
#include <memory.h>
#include <string>
#include <stdexcept>

namespace ilzn
{
       using byte = unsigned char;

       enum class PicType
       {
              PNG,
              JPG,
              BMP,
              PSD,
              TGA,
              GIF,
              HDR,
              PIC,
              PNM
       };

       std::string get_extension_name_from_picture_type(PicType pic_type);

       PicType get_picture_type_from_extension_name(const std::string &ext_name);

       bool is_a_picture(const std::string &file, size_t &ext_pos);

       class Picture
       {
              void *m_data;
              int m_width, m_height;

       public:
              Picture();
              ~Picture();
              void load(const std::string &file_name);
              void alloc(int width, int height);
              void alloc(const Picture &picture);
              void write(const std::string &file_name, PicType pic_type);
              void clear();
              void copy(const Picture &picture);
              byte &operator[](size_t index);
              byte operator[](size_t index) const;
              inline int width() const noexcept
              {
                     return m_width;
              }
              inline int height() const noexcept
              {
                     return m_height;
              }
       };

       namespace effect
       {
              /**
               * picture : base picture
               * result : picture allocated with the same size of 'picture'
               */
              void grey_scale(const Picture& picture, Picture &result);

              /**
               * picture : base picture
               * result : picture allocated with the same size of 'picture'
               */
              void invert(const Picture& picture, Picture &result);

              /**
               * picture : base picture
               * result : picture allocated with the same size of 'picture'
               */
              void flip_horizontal(const Picture& picture, Picture &result);

              /**
               * picture : base picture
               * result : picture allocated with the prefered size
               */
              void resize(const Picture& picture, Picture &result);
       }
}

#endif //__ILLUZION_H__