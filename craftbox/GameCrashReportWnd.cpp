#include "GameCrashReportWnd.h"
#include "glog/logging.h"

GameCrashReportWnd::GameCrashReportWnd()
	: BaseShadowWindow(L"gamecrash.xml")
{
}

GameCrashReportWnd::~GameCrashReportWnd()
{
}

void GameCrashReportWnd::setGameDir(const std::wstring &base_dir, const std::wstring &version_dir)
{
	base_dir_ = base_dir;
	version_dir_ = version_dir;
}

LRESULT GameCrashReportWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	auto pControl = this->GetPaintManager()->FindControl(pt);
	if (pControl == nullptr)
		return HTCAPTION;
	LPCTSTR class_name = pControl->GetClass();
	if (_tcscmp(class_name, DUI_CTR_TABLAYOUT) == 0
		|| _tcscmp(class_name, DUI_CTR_TILELAYOUT) == 0
		|| _tcscmp(class_name, DUI_CTR_VERTICALLAYOUT) == 0
		|| _tcscmp(class_name, DUI_CTR_HORIZONTALLAYOUT) == 0
		|| _tcscmp(class_name, DUI_CTR_LABEL) == 0
		)
		return HTCAPTION;

	return HTCLIENT;
}

void GameCrashReportWnd::InitWindow()
{
	close_btn_ = (DuiLib::CButtonUI*)this->GetPaintManager()->FindControl(L"Close");
	ok_btn_ = (DuiLib::CButtonUI*)this->GetPaintManager()->FindControl(L"ok");
	note_edit_ = (DuiLib::CRichEditUI*)this->GetPaintManager()->FindControl(L"note_edit");
	game_log_btn_ = (DuiLib::CButtonUI *)this->GetPaintManager()->FindControl(L"open_log_dir");
	DCHECK(note_edit_ && close_btn_ && ok_btn_ && game_log_btn_);

	std::wstring text = L"�ǳ���Ǹ, �ҵ����� \"" + version_dir_ + L"\" ��Ϸ���̱����ˡ�����Գ������·������������:\r\n1�� ����Java�汾: ��Java1.7�л���Java1.8, ���ߴ�Java1.8�л���Java1.7\r\n2�� 32λϵͳ��, �뾡����Java���������ڴ����1024M����\r\n3�� ����Ϸ��־�ж�λ����";
	
	LONG lineSpace = (LONG)(0.5 * 20);//XΪҪ���õ��м��
	PARAFORMAT2 pf;
	ZeroMemory(&pf, sizeof(pf));
	pf.cbSize = sizeof(PARAFORMAT2);
	pf.dwMask |= PFM_LINESPACING;
	pf.bLineSpacingRule = 5;
	pf.dyLineSpacing = lineSpace;
	note_edit_->SetParaFormat(pf);

	note_edit_->SetText(text.c_str());
}

void GameCrashReportWnd::Notify(DuiLib::TNotifyUI& msg)
{
	if (msg.sType == L"click")
	{
		if (msg.pSender == close_btn_)
		{
			Close();
		}
		else if (msg.pSender == ok_btn_)
		{
			Close();
		}
		else if (msg.pSender == game_log_btn_)
		{
			::ShellExecuteW(this->GetHWND(), L"open", std::wstring(base_dir_ + L"\\versions\\"+version_dir_+L"\\logs").c_str(), nullptr, nullptr, SW_SHOW);
		}
	}
	else
		__super::Notify(msg);
}
