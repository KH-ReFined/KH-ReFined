#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define DLL_EXPORT __declspec(dllexport)

#include "obj2d.h"
#include "sprite.h"
#include "sequence.h"
#include "converter.h"
#include "binarc.h"
#include "image.h"
#include "field2dd.h"
#include "file.h"
#include "info_base.h"
#include "image_cache.h"
#include "memorymgr.h"

extern "C"
{
    namespace dk
    {
        class DLL_EXPORT NEXT_FORM
        {
        public:
            char m_infobase[0x38];
            char m_image[0x100];
            char m_labelSeq[0x1A0];
            char m_numberSeq[0xD00];
            short m_numberOffset[0x08];
            short m_numberCount;

            int m_numberCurrent;
            int m_numberDisplay;

            int m_formCurrent;

            int offset16x9;

            char* vftable[0x05];

            NEXT_FORM()
            {
                char region_filename[0x80];

                memset(this, 0x00, sizeof(dk::NEXT_FORM));

                auto _funcUpdate = &dk::NEXT_FORM::update;
                auto _funcDraw = &dk::NEXT_FORM::draw;
                auto _funcLeave = &dk::NEXT_FORM::release;


                memcpy(vftable, dk::Obj2D::vftable, 0x28);
                memset(vftable + 1, 0x00, 0x18);

                memcpy(vftable + 1, &_funcUpdate, 0x08);
                memcpy(vftable + 2, &_funcDraw, 0x08);
                memcpy(vftable + 3, &_funcLeave, 0x08);

                *reinterpret_cast<char***>(m_infobase) = vftable;

                sprintf_s(region_filename, 0x80, m_filename, reinterpret_cast<char*>(*YS::REGION::Region));

                if (YS::FILE::GetSize(region_filename))
                {
                    YS::FILE::ReadBAR(region_filename, m_layoutFile);

                    m_imd = PC::CONVERTER::INTPTR_TO_POINTER(*reinterpret_cast<uint32_t*>(YS::BINARC::get_info_by_tag(m_layoutFile, 0x18, 0x6C766C6E, 0) + 0x08));
                    m_sqd = PC::CONVERTER::INTPTR_TO_POINTER(*reinterpret_cast<uint32_t*>(YS::BINARC::get_info_by_tag(m_layoutFile, 0x19, 0x6C766C6E, 0) + 0x08));

                    YI::IMAGE::_IMAGE(m_image);
                    YI::SEQUENCE::_SEQUENCE(m_labelSeq);

                    for (int i = 0x00; i < 0x08; i++)
                        YI::SEQUENCE::_SEQUENCE(m_numberSeq + 0x1A0 * i);
                }
            }

            void draw()
            {
                YI::IMAGE::LoadTexture(m_image);

                int _spriteX = YI::SEQUENCE::GetActiveX(m_labelSeq) + offset16x9;
                int _spriteY = YI::SEQUENCE::GetActiveY(m_labelSeq) + *reinterpret_cast<float*>(m_infobase + 0x30);

                *reinterpret_cast<int*>(m_labelSeq + 0x18C) = offset16x9;
                *reinterpret_cast<int*>(m_labelSeq + 0x190) = *reinterpret_cast<float*>(m_infobase + 0x30);

                YI::SEQUENCE::Draw(m_labelSeq);

                for (int i = 0; i < m_numberCount; i++)
                {
                    *reinterpret_cast<int*>(m_numberSeq + 0x18C + 0x1A0 * i) = m_numberOffset[i] + _spriteX;
                    *reinterpret_cast<int*>(m_numberSeq + 0x190 + 0x1A0 * i) = YI::SEQUENCE::GetParamEx(m_labelSeq) + _spriteY;

                    YI::SEQUENCE::Draw(m_numberSeq + 0x1A0 * i);
                }
            }

            void update()
            {
                if (m_numberCurrent != m_numberDisplay)
                    this->updateNumber();

                dk::INFO_BASE::update(m_infobase);

                if (*(m_infobase + 0x018) & 1)
                    reinterpret_cast<void(*)(dk::NEXT_FORM*)>(m_infobase + 0x20)(this);

                YI::SEQUENCE::Control(m_labelSeq);

                for (int i = 0; i < m_numberCount; i++)
                    YI::SEQUENCE::Control(m_numberSeq + 0x1A0 * i);

                if (*(m_numberSeq + 0x140) == 0x03)
                    release();
            }

            void updateNumber()
            {
                char _numberSequence[0x10];
                memset(_numberSequence, 0x00, 0x10);

                sprintf_s(_numberSequence, 0x10, "%d", m_numberCurrent);

                int _numberOffsetX = YI::SEQUENCE::GetParamSize(m_labelSeq);

                for (int i = 0x00; i < 0x09; i++)
                {
                    if (!_numberSequence[i])
                    {
                        m_numberCount = i;
                        break;
                    }

                    _numberSequence[i] = (_numberSequence[i] - 0x30) + (m_formCurrent == -1 ? 0x21 : (0x0B * m_formCurrent));
                    YI::SEQUENCE::SetNumberForce(m_numberSeq + 0x1A0 * i, _numberSequence[i]);

                    *(m_numberOffset + i) = _numberOffsetX;
                    _numberOffsetX += YI::SEQUENCE::GetParamCr(m_numberSeq + 0x1A0 * i);
                }

                m_numberDisplay = m_numberCurrent;
            }

            void create(int form, int number, int offset16x9 = -85)
            {
                if (offset16x9 != this->offset16x9 && offset16x9 != -1)
                    this->offset16x9 = offset16x9;

                this->m_numberCurrent = number;
                this->m_formCurrent = form - 1;

                if (!dk::Obj2D::isExist(m_infobase) && *reinterpret_cast<uint32_t*>(m_infobase + 0x08) == 0x00)
                {
                    m_numberDisplay = -1;

                    YI::IMAGE::Init(m_image, m_imd);

                    YI::IMAGE::MakePacket(m_image);
                    YI::IMAGE::InitLoadImage(m_image);

                    YI::SEQUENCE::Init(m_labelSeq, m_sqd, m_image);
                    YI::SEQUENCE::SetNumberForce(m_labelSeq, m_formCurrent == -1 ? 0x37 : 0x0A + 0x0B * m_formCurrent);

                    for (int i = 0x00; i < 0x08; i++)
                        YI::SEQUENCE::Init(m_numberSeq + 0x1A0 * i, m_sqd, m_image);

                    dk::INFO_BASE::create(m_infobase, 0x0F, YI::SEQUENCE::GetParamCr(m_labelSeq), 15000, 7, 0);
                }

                else
                    YI::SEQUENCE::SetNumberForce(m_labelSeq, m_formCurrent == -1 ? 0x37 : 0x0A + 0x0B * m_formCurrent);
            }

            void release()
            {
                *reinterpret_cast<uint16_t*>(dk::INFO_BASE::sm_height + 0x06 * 0x0F) = 0x00;

                for (int i = 0x07; i >= 0x00; i--)
                    YI::SEQUENCE::Dispose(m_numberSeq + 0x1A0 * i);

                YI::SEQUENCE::Dispose(m_labelSeq);

                *reinterpret_cast<uint32_t*>(m_infobase + 0x10) |= 0x01;
            }

        protected:
            static const inline char* m_filename = "field2d/%s/zz0level.2dd";

            char* m_imd;
            char* m_sqd;

            char m_layoutFile[0x30000];
        };
    }
}