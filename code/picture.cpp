#include "illuzion.hpp"

#include "stb_image.h"
#include "stb_image_write.h"

#include <iostream>

namespace ilzn
{
       std::string get_extension_name_from_picture_type(PicType pic_type)
       {
              switch (pic_type)
              {
              case PicType::PNG:
                     return ".png";
              case PicType::JPG:
                     return ".jpg";
              case PicType::BMP:
                     return ".bmp";
              case PicType::PSD:
                     return ".psd";
              case PicType::TGA:
                     return ".tga";
              case PicType::GIF:
                     return ".gif";
              case PicType::HDR:
                     return ".hdr";
              case PicType::PIC:
                     return ".pic";
              case PicType::PNM:
                     return ".pnm";
              default:
                     throw std::runtime_error("picture type unknown");
              }
       }

       PicType get_picture_type_from_extension_name(const std::string &ext_name)
       {
              if (ext_name == ".png")
              {
                     return PicType::PNG;
              }
              else if (ext_name == ".jpg")
              {
                     return PicType::JPG;
              }
              else if (ext_name == ".bmp")
              {
                     return PicType::BMP;
              }
              else if (ext_name == ".psd")
              {
                     return PicType::PSD;
              }
              else if (ext_name == ".tga")
              {
                     return PicType::TGA;
              }
              else if (ext_name == ".gif")
              {
                     return PicType::GIF;
              }
              else if (ext_name == ".hdr")
              {
                     return PicType::HDR;
              }
              else if (ext_name == ".pic")
              {
                     return PicType::PIC;
              }
              else if (ext_name == ".pnm")
              {
                     return PicType::PNM;
              }else
              {
                     throw std::runtime_error("extension name unknwon");
              }
       }

       bool is_a_picture(const std::string &file, size_t &ext_pos)
       {
              return (ext_pos = file.find(get_extension_name_from_picture_type(PicType::PNG))) != std::string::npos ||
                     (ext_pos = file.find(get_extension_name_from_picture_type(PicType::JPG))) != std::string::npos ||
                     (ext_pos = file.find(get_extension_name_from_picture_type(PicType::BMP))) != std::string::npos ||
                     (ext_pos = file.find(get_extension_name_from_picture_type(PicType::PSD))) != std::string::npos ||
                     (ext_pos = file.find(get_extension_name_from_picture_type(PicType::TGA))) != std::string::npos ||
                     (ext_pos = file.find(get_extension_name_from_picture_type(PicType::GIF))) != std::string::npos ||
                     (ext_pos = file.find(get_extension_name_from_picture_type(PicType::HDR))) != std::string::npos ||
                     (ext_pos = file.find(get_extension_name_from_picture_type(PicType::PIC))) != std::string::npos ||
                     (ext_pos = file.find(get_extension_name_from_picture_type(PicType::PNM))) != std::string::npos;
       }

       Picture::Picture() : m_data(nullptr), m_width(0), m_height(0)
       {
       }

       Picture::~Picture()
       {
              clear();
       }

       void Picture::load(const std::string &file_name)
       {
              if (m_data != nullptr)
              {
                     clear();
              }
              int channels;
              m_data = stbi_load(file_name.c_str(), &m_width, &m_height, &channels, STBI_rgb_alpha);
              if (!m_data)
              {
                     throw std::runtime_error("failed to load picture : " + std::string(file_name));
              }
       }

       void Picture::alloc(int width, int height)
       {
              if (m_data != nullptr)
              {
                     clear();
              }
              m_data = malloc(width * height * 4);
              if (m_data == nullptr)
              {
                     throw std::runtime_error("failed to alloc memory for picture");
              }
              m_width = width;
              m_height = height;
       }

       void Picture::alloc(const Picture &picture)
       {

              alloc(picture.width(), picture.height());
       }

       void Picture::write(const std::string &file_name, PicType pic_type)
       {
              std::string picture_name = (file_name + get_extension_name_from_picture_type(pic_type));
              switch (pic_type)
              {
              case PicType::PNG:
                     if (!stbi_write_png(picture_name.c_str(), m_width, m_height, 4, m_data, 4 * m_width))
                     {
                            throw std::runtime_error("failed to write picture : " + std::string(file_name));
                     }
                     return;
              case PicType::BMP:
                     if (!stbi_write_bmp(picture_name.c_str(), m_width, m_height, 4, m_data))
                     {
                            throw std::runtime_error("failed to write picture : " + std::string(file_name));
                     }
                     return;
              case PicType::TGA:
                     if (!stbi_write_tga(picture_name.c_str(), m_width, m_height, 4, m_data))
                     {
                            throw std::runtime_error("failed to write picture : " + std::string(file_name));
                     }
                     return;
              case PicType::JPG:
                     if (!stbi_write_jpg(picture_name.c_str(), m_width, m_height, 4, m_data, 4 * m_width))
                     {
                            throw std::runtime_error("failed to write picture : " + std::string(file_name));
                     }
                     return;

              default:
                     throw std::runtime_error("picture type is not valid");
              }
       }

       void Picture::clear()
       {
              free(m_data);
              m_data = nullptr;
              m_width = 0;
              m_height = 0;
       }

       void Picture::copy(const Picture &picture)
       {
              alloc(picture);
              memcpy(m_data, picture.m_data, picture.width() * picture.height() * 4);
       }

       byte &Picture::operator[](size_t index)
       {
              return ((byte *)m_data)[index];
       }

       byte Picture::operator[](size_t index) const
       {
              return ((byte *)m_data)[index];
       }
}