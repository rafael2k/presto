/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
 *
 * Copyright (C) 1995-2007 Opera Software AS.  All rights reserved.
 *
 * This file is part of the Opera web browser.	It may not be distributed
 * under any circumstances.
 *
 * @author Markus Johansson
*/

#include "core/pch.h"

#ifdef SESSION_SUPPORT

#include "opsession.h"
#include "modules/util/opfile/opfile.h"
#include "modules/prefsfile/prefsfile.h"
#include "modules/util/excepts.h"
#include "modules/util/opfile/opfile.h"
#include "modules/util/opfile/opsafefile.h"
#include "modules/prefsfile/prefsentry.h"
#include "modules/prefsfile/prefssection.h"
#include "modules/prefsfile/prefsfile.h"
#include "modules/prefsfile/impl/backend/prefssectioninternal.h"
#include "modules/prefsfile/impl/backend/prefsmap.h"

/***********************************************************************************
**
** SessionFileAccessor - Be a bit more relaxed on the safety of the file for
**					performance reasons - see DSK-336599
**
***********************************************************************************/
OP_STATUS 
OpSessionReader::SessionFileAccessor::StoreL(OpFileDescriptor *file, const PrefsMap *prefsMap)
{
	// If this is a disk file, we need to use a OpSafeFile to rewrite
	// it, to ensure we don't lose data if writing fails.
	OpStackAutoPtr<OpSafeFile> safefile(NULL);
	if (file->Type() == OPFILE)
	{
		safefile.reset(OP_NEW_L(OpSafeFile, ()));
		LEAVE_IF_ERROR(safefile->Construct(static_cast<OpFile *>(file)));
		file = safefile.get();
	}
	LEAVE_IF_ERROR(file->Open(OPFILE_WRITE | OPFILE_TEXT));
	LEAVE_IF_ERROR(file->WriteUTF8Line(UNI_L("\xFEFFOpera Preferences version 2.1")));
	LEAVE_IF_ERROR(file->WriteUTF8Line(UNI_L("; Do not edit this file while Opera is running")));
	LEAVE_IF_ERROR(file->WriteUTF8Line(UNI_L("; This file is stored in UTF-8 encoding")));

	const PrefsSection *section = prefsMap->Sections();
	while (section != NULL)
	{
		StoreSectionL(file, section);
		section = section->Suc();
	}
	if (safefile.get())
	{
		LEAVE_IF_ERROR(safefile->Close());
	}
	return OpStatus::OK;

}

// OpSessionReader

/***********************************************************************************
**
** OpSessionReader - Constructor
**
***********************************************************************************/
OpSessionReader::OpSessionReader() :
	m_prefsfile(NULL)
{
}

/***********************************************************************************
**
** OpSessionReader::InitL
**
***********************************************************************************/
void OpSessionReader::InitL(const OpStringC8& section, PrefsFile* prefsfile)
{
	// NULL means we'll create the default session file here
	if(!prefsfile)
	{
		OpStackAutoPtr<SessionFileAccessor> accessor(OP_NEW_L(SessionFileAccessor, ()));

		// we need to open the session in prefsManager->GetWindowStorageFile()
		OpStackAutoPtr<PrefsFile> file(OP_NEW_L(PrefsFile, (accessor.release())));
		
		file->ConstructL();

		OpFile winstore;
		g_pcfiles->GetFileL(PrefsCollectionFiles::WindowsStorageFile, winstore);
		file->SetFileL(&winstore);
		prefsfile = file.release();
	}
	m_prefsfile = prefsfile;
	m_section.SetL(section);
}

bool OpSessionReader::HasValue(const OpStringC8& name) const
{
	return m_prefsfile->IsKey(m_section, name) != FALSE;
}

/***********************************************************************************
**
** OpSessionReader::GetValueL
**
***********************************************************************************/
INT32 OpSessionReader::GetValueL(const OpStringC8& name) const
{
	return m_prefsfile->ReadIntL(m_section, name);
}

/***********************************************************************************
**
** OpSessionReader::SetValueL
**
***********************************************************************************/
void OpSessionReader::SetValueL(const OpStringC8& name, INT32 value)
{
	m_prefsfile->WriteIntL(m_section, name, value);
}

/***********************************************************************************
**
** OpSessionReader::GetStringL
**
***********************************************************************************/
const OpStringC OpSessionReader::GetStringL(const OpStringC8& name) const
{
	return m_prefsfile->ReadStringL(m_section, name);
}

/***********************************************************************************
**
** OpSessionReader::SetStringL
**
***********************************************************************************/
void OpSessionReader::SetStringL(const OpStringC8& name, const OpStringC& string)
{
	m_prefsfile->WriteStringL(m_section, name, string);
}

/***********************************************************************************
**
** OpSessionReader::ClearStringArrayL
**
***********************************************************************************/
void OpSessionReader::ClearStringArrayL()
{
	SetStringL( "lists", UNI_L("") );
}

/***********************************************************************************
**
** OpSessionReader::SetStringArrayL
**
***********************************************************************************/
void OpSessionReader::SetStringArrayL(const OpStringC8& name, const OpVector<OpString>& array)
{
	if (array.GetCount() == 0)
		return;

	OpString lists; ANCHOR(OpString, lists);
	m_prefsfile->ReadStringL(m_section, "lists", lists);
	if (lists.HasContent())
		lists.AppendL(",");
	lists.AppendL(name.CStr());
	m_prefsfile->WriteStringL(m_section, "lists", lists);

	OpString8 array_section; ANCHOR(OpString8, array_section);
	array_section.SetL(m_section);
	array_section.AppendL(name);
	m_prefsfile->WriteIntL(array_section, "count", array.GetCount());

	for (UINT32 i=0; i < array.GetCount(); ++i)
	{
		OpString8 key; ANCHOR(OpString8, key);
		LEAVE_IF_ERROR(key.AppendFormat("%i", i));

		m_prefsfile->WriteStringL(array_section, key, *array.Get(i));
	}
}

/***********************************************************************************
**
** OpSessionReader::GetStringArrayL
**
***********************************************************************************/
void OpSessionReader::GetStringArrayL(const OpStringC8& name, OpVector<OpString>& array) const
{
	OpString8 array_section; ANCHOR(OpString8, array_section);

	array_section.SetL(m_section);
	array_section.AppendL(name);

	UINT32 count = m_prefsfile->ReadIntL(array_section, "count");

	for (UINT32 i=0; i < count; ++i)
	{
		OpString8 key; ANCHOR(OpString8, key);
		LEAVE_IF_ERROR(key.AppendFormat("%i", i));

		OpStringC item = m_prefsfile->ReadStringL(array_section, key);
		OpString* item_copy = OP_NEW_L(OpString, ());
		OpStackAutoPtr<OpString> item_copy_ap(item_copy);
		item_copy->SetL(item);
		LEAVE_IF_ERROR(array.Add(item_copy));
		item_copy_ap.release();
	}
}

/***********************************************************************************
**
** OpSessionReader::SetValueArrayL
**
***********************************************************************************/
void OpSessionReader::SetValueArrayL(const OpStringC8& name, const OpVector<UINT32>& array)
{
	if (array.GetCount() == 0)
		return;

	OpString lists; ANCHOR(OpString, lists);
	m_prefsfile->ReadStringL(m_section, "lists", lists);
	if (lists.HasContent())
		lists.AppendL(",");
	lists.AppendL(name.CStr());
	m_prefsfile->WriteStringL(m_section, "lists", lists);

	OpString8 array_section; ANCHOR(OpString8, array_section);
	array_section.SetL(m_section);
	array_section.AppendL(name);
	m_prefsfile->WriteIntL(array_section, "count", array.GetCount());

	for (UINT32 i=0; i < array.GetCount(); ++i)
	{
		OpString8 key; ANCHOR(OpString8, key);
		LEAVE_IF_ERROR(key.AppendFormat("%i", i));

		m_prefsfile->WriteIntL(array_section, key, *array.Get(i));
	}
}

/***********************************************************************************
**
** OpSessionReader::GetValueArrayL
**
***********************************************************************************/
void OpSessionReader::GetValueArrayL(const OpStringC8& name, OpVector<UINT32>& array) const
{
	OpString8 array_section; ANCHOR(OpString8, array_section);

	array_section.SetL(m_section);
	array_section.AppendL(name);

	UINT32 count = m_prefsfile->ReadIntL(array_section, "count");

	for (UINT32 i=0; i < count; ++i)
	{
		OpString8 key; ANCHOR(OpString8, key);
		LEAVE_IF_ERROR(key.AppendFormat("%i", i));

		UINT32 item = m_prefsfile->ReadIntL(array_section, key);
		UINT32* item_copy = OP_NEW_L(UINT32, ());
		*item_copy = item;

		OP_STATUS status = array.Add(item_copy);
		if (OpStatus::IsError(status))
		{
			OP_DELETE(item_copy);
			LEAVE(status);
		}
	}
}

// OpSession
/***********************************************************************************
**
** OpSession - Destructor
**
***********************************************************************************/
OpSession::~OpSession()
{
	// must be here and not in OpSessionReader as it gets initialized with an input file 
	// that is shared among multiple OpSessionWindows. 
	OP_DELETE(m_prefsfile);
}

/***********************************************************************************
**
** OpSession::InitL
**
***********************************************************************************/
void OpSession::InitL(PrefsFile* prefsfile, BOOL in_memory_only)
{
	m_in_memory_only = in_memory_only;
	OpSessionReader::InitL("session", prefsfile);
}

/***********************************************************************************
**
** OpSession::LoadL
**
***********************************************************************************/
void OpSession::LoadL()
{
	m_prefsfile->LoadAllL();
}

/***********************************************************************************
**
** OpSession::GetVersionL
**
***********************************************************************************/
UINT32 OpSession::GetVersionL() const
{
	return m_prefsfile->ReadIntL("session", "version", 0);
}

/***********************************************************************************
**
** OpSession::SetVersionL
**
***********************************************************************************/
void OpSession::SetVersionL(UINT32 version)
{
	m_prefsfile->WriteIntL("session", "version", version);
}

/***********************************************************************************
**
** OpSession::GetCreationDateL
**
***********************************************************************************/
UINT32 OpSession::GetCreationDateL() const
{
	return m_prefsfile->ReadIntL("session", "creation date");
}

/***********************************************************************************
**
** OpSession::SetCreationDateL
**
***********************************************************************************/
void OpSession::SetCreationDateL(UINT32 creation_date)
{
	m_prefsfile->WriteIntL("session", "creation date", creation_date);
}

/***********************************************************************************
**
** OpSession::GetCommentL
**
***********************************************************************************/
OpStringC OpSession::GetCommentL() const
{
	return m_prefsfile->ReadStringL("session", "comment");
}

/***********************************************************************************
**
** OpSession::SetCommentL
**
***********************************************************************************/
void OpSession::SetCommentL(const OpStringC& comment)
{
	m_prefsfile->WriteStringL("session", "comment", comment);
}

/***********************************************************************************
**
** OpSession::GetWindowCountL
**
***********************************************************************************/
UINT32 OpSession::GetWindowCountL() const
{
	return m_prefsfile->ReadIntL("session", "window count");
}

/***********************************************************************************
**
** OpSession::GetWindowL
**
***********************************************************************************/
OpSessionWindow* OpSession::GetWindowL(UINT32 index)
{
	if (m_windows.GetCount() == 0 && !m_in_memory_only) // m_windows has not been initialized
	{
		UINT32 count = m_prefsfile->ReadIntL("session", "window count");

		UINT32 i = 0;
		while (i < count)
		{
			OpSessionWindow* window = OP_NEW_L(OpSessionWindow, ());
			OP_STATUS err = m_windows.Add(window);
			if (OpStatus::IsError(err))
			{
				OP_DELETE(window);
				LEAVE(err);
			}
			window->InitL(m_prefsfile, ++i);
		}
	}

	return m_windows.Get(index);
}

/***********************************************************************************
**
** OpSession::AddWindowL
**
***********************************************************************************/
OpSessionWindow* OpSession::AddWindowL()
{
	static INT32 forever_counter = 0;

	OpSessionWindow *window = OP_NEW_L(OpSessionWindow, ());
	OpStackAutoPtr<OpSessionWindow> window_ap(window);

	UINT32 newcount = m_windows.GetCount()+1;
	window->InitL(m_prefsfile, m_in_memory_only ? forever_counter++ : newcount);
	m_prefsfile->WriteIntL("session", "window count", newcount);
	LEAVE_IF_ERROR(m_windows.Add(window));

	return window_ap.release();
}

/***********************************************************************************
**
** OpSession::DeleteWindowL
**
***********************************************************************************/
void OpSession::DeleteWindowL(UINT32 index)
{
	OpSessionWindow* window = m_windows.Get(index);
	DeleteWindowL(window);
}

/***********************************************************************************
**
** OpSession::DeleteWindowL
**
***********************************************************************************/
void OpSession::DeleteWindowL(OpSessionWindow* window)
{
	if (window)
	{
		window->DeleteL();
		m_windows.Delete(window);
		m_prefsfile->WriteIntL("session", "window count", m_windows.GetCount());
	}
}

/***********************************************************************************
**
** OpSession::WriteToFileL
**
***********************************************************************************/
void OpSession::WriteToFileL(BOOL reset_after_write)
{
	m_prefsfile->CommitL(FALSE, TRUE);

	if(reset_after_write)
	{
		OP_DELETE(m_prefsfile);
		m_prefsfile = NULL;
	}
}

/***********************************************************************************
**
** OpSession::Cancel
**
***********************************************************************************/
void OpSession::Cancel()
{
	if(m_prefsfile)
	{
		// ensure it's not written to in the destructor by just aborting it here
		m_prefsfile->Reset();
		OP_DELETE(m_prefsfile);
		m_prefsfile = NULL;
	}
}

/***********************************************************************************
**
** OpSession::DeleteL
**
***********************************************************************************/
BOOL OpSession::DeleteL()
{
	if(m_prefsfile->GetFile())
	{
		((OpFile*)m_prefsfile->GetFile())->Delete();
		return TRUE;
	}
	return FALSE;
}

/***********************************************************************************
**
** OpSession::operator==
**
***********************************************************************************/
BOOL OpSession::operator==(const OpSession* filetocompare)
{
	const uni_char* thispath = ((OpFile*)m_prefsfile->GetFile())->GetFullPath();
	const uni_char* thatpath = ((OpFile*)filetocompare->m_prefsfile->GetFile())->GetFullPath();
	if(thispath && thatpath)
		return ( uni_stricmp(thispath, thatpath) == 0);
	else
		return FALSE;
}

/***********************************************************************************
**
** OpSession::InitForWrite - 
**
***********************************************************************************/
OP_STATUS OpSession::InitForWrite()
{	
	OP_STATUS s = OpStatus::OK;

	// we don't want cascading when writing this session
	TRAP(s, m_prefsfile->DeleteAllSectionsL());

	return s;
}

// OpSessionWindow

/***********************************************************************************
**
** OpSessionWindow::InitL
**
***********************************************************************************/
void OpSessionWindow::InitL(PrefsFile* prefsfile, UINT32 id)
{
	OpString8 section; ANCHOR(OpString8, section);
	LEAVE_IF_ERROR(section.AppendFormat("%i", id));

	OpSessionReader::InitL(section, prefsfile);
}

/***********************************************************************************
**
** OpSessionWindow::GetTypeL
**
***********************************************************************************/
OpSessionWindow::Type OpSessionWindow::GetTypeL() const
{
	return (Type)m_prefsfile->ReadIntL(m_section, "type");
}

/***********************************************************************************
**
** OpSessionWindow::SetTypeL
**
***********************************************************************************/
void OpSessionWindow::SetTypeL(Type type)
{
	m_prefsfile->WriteIntL(m_section, "type", type);
}

/***********************************************************************************
**
** OpSessionWindow::DeleteL
**
***********************************************************************************/
void OpSessionWindow::DeleteL()
{
	OpString lists; ANCHOR(OpString, lists);
	m_prefsfile->ReadStringL(m_section, "lists", lists);

	OpString8 tmp; ANCHOR(OpString8, tmp);
	if (lists.HasContent())
	{
		uni_char* p = lists.CStr();

		while(true)
		{
			uni_char* q = uni_strstr(p, UNI_L(","));
			if (q)
			{
				*q = 0;
			}

			tmp.SetL(p);
			tmp.InsertL(0, m_section);
			m_prefsfile->DeleteSectionL(tmp.CStr());

			if (q)
			{
				p = q + 1;
			}
			else
			{
				break;
			}
		}
	}

	tmp.SetL(m_section);
	m_prefsfile->DeleteSectionL(tmp.CStr());
}

#endif // SESSION_SUPPORT