/*
 * gui_toplevel.cpp
 *
 *  Created on: Nov 4, 2008
 *      Author: gheckler
 */

#include "gui.h"

DECLARE_APP(GUI_App)

/*----------------------------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(GUI_Ephemeris, wxFrame)
    EVT_CLOSE(GUI_Ephemeris::onClose)
    EVT_LEFT_DOWN(GUI_Ephemeris::onMouse)
END_EVENT_TABLE()
/*----------------------------------------------------------------------------------------------*/

GUI_Ephemeris::GUI_Ephemeris():iGUI_Ephemeris(NULL, wxID_ANY, wxT("Ephemeris"), wxDefaultPosition, wxSize(400,600), wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL)
{

	sv = 0;

}

GUI_Ephemeris::~GUI_Ephemeris()
{


}

void GUI_Ephemeris::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}

void GUI_Ephemeris::render(wxDC& dc)
{
	renderDecoded();
	renderSV();
}


void GUI_Ephemeris::onMouse(wxMouseEvent& event)
{

	wxRect rect;
	wxPoint br, tl;
	int32 mx, my;
	int32 row, col;
	int32 dX,dY;

	tl = pDecoded->GetPosition();
	rect = pDecoded->GetClientRect();
	br = rect.GetBottomRight();

	dY = (br.y/4);
	dX = (4*br.x/NUM_CODES);

	br.x += tl.x;
	br.y += tl.y;

	mx = wxGetMousePosition().x - this->GetScreenPosition().x;
	my = wxGetMousePosition().y - this->GetScreenPosition().y;

	/* Within the pDecoded area */
	if((mx >= tl.x) && (mx <= br.x) && (my >= tl.y) && (my <= br.y))
	{
		mx = mx - tl.x;
		my = my - tl.y;

		row = mx/dX;
		col = my/dY;
		sv = row;
		sv += col*(NUM_CODES>>2);

		pSerial->formCommand(GET_EPHEMERIS_C_ID, &sv);
	}
}


void GUI_Ephemeris::renderDecoded()
{

	int32 dX, dY, lcv;

	wxPoint box[4];
	wxString str;
	wxPaintDC dc(pDecoded);
	dc.Clear();

	wxCoord w, h;
	dc.GetSize(&w, &h);

	dY = (h/4);
	dX = (4*w/NUM_CODES);

	box[0].x = 0;	box[0].y = 0;
	box[1].x = dX;	box[1].y = 0;
	box[2].x = dX;	box[2].y = dY;
	box[3].x = 0;	box[3].y = dY;

	/* Render the decoded ephemerides */
	for(lcv = 0; lcv < NUM_CODES; lcv++)
	{
		if(p->ephemeris_status.valid[lcv])
		{
			dc.SetBrush(wxBrush(wxColor(0,255,0)));
			dc.SetPen(wxPen(wxColor(0,0,0), 1));
		}
		else
		{
			dc.SetBrush(wxBrush(wxColor(127,127,127)));
			dc.SetPen(wxPen(wxColor(0,0,0), 1));
		}
		dc.DrawPolygon(4, box, (lcv%(NUM_CODES>>2))*dX, (lcv/(NUM_CODES>>2))*dY);

		str.Printf(wxT("%02d"),(int)lcv+1);
		dc.DrawText(str, (lcv%(NUM_CODES>>2))*dX + dX/3, (lcv/(NUM_CODES>>2))*dY + dY/5);
	}

}

void GUI_Ephemeris::renderSV()
{

	wxString str;
	tDisplay->Clear();

	Ephemeris_M *e;
	e = &p->ephemerides[sv];
	str.Printf(wxT("************* Ephemeris for PRN-%02d *************\n"),sv+1);
	tDisplay->AppendText(str);
	str.Printf(wxT("ID:                        %02d\n"),sv+1);
	tDisplay->AppendText(str);
	str.Printf(wxT("Health:                    %03d\n"),e->valid);
	tDisplay->AppendText(str);
	str.Printf(wxT("Eccentricity:              %.10g\n"),e->ecc);
	tDisplay->AppendText(str);
	str.Printf(wxT("Time of Ephemeris(s):      %.10g\n"),e->toe);
	tDisplay->AppendText(str);
	str.Printf(wxT("Orbital Inclination(rad):  %.10g\n"),e->in0);
	tDisplay->AppendText(str);
	str.Printf(wxT("Inclination Rate(r/s):     %.10g\n"),e->idot);
	tDisplay->AppendText(str);
	str.Printf(wxT("Rate of Right Ascen(r/s):  %.10g\n"),e->omd);
	tDisplay->AppendText(str);
	str.Printf(wxT("SQRT(A)  (m 1/2):          %.10g\n"),e->sqrta);
	tDisplay->AppendText(str);
	str.Printf(wxT("Right Ascen at Week(rad):  %.10g\n"),e->om0);
	tDisplay->AppendText(str);
	str.Printf(wxT("Argument of Perigee(rad):  %.10g\n"),e->argp);
	tDisplay->AppendText(str);
	str.Printf(wxT("Mean Anom(rad):            %.10g\n"),e->m0);
	tDisplay->AppendText(str);
	str.Printf(wxT("Mean Motion(rad):          %.10g\n"),e->deltan);
	tDisplay->AppendText(str);
	str.Printf(wxT("Time of Clock(s):          %.10g\n"),e->toc);
	tDisplay->AppendText(str);
	str.Printf(wxT("Af0(s):                    %.10g\n"),e->af0);
	tDisplay->AppendText(str);
	str.Printf(wxT("Af1(s/s):                  %.10g\n"),e->af1);
	tDisplay->AppendText(str);
	str.Printf(wxT("Af2(s/s/s):                %.10g\n"),e->af2);
	tDisplay->AppendText(str);
	str.Printf(wxT("CIC:                       %.10g\n"),e->cic);
	tDisplay->AppendText(str);
	str.Printf(wxT("CIS:                       %.10g\n"),e->cis);
	tDisplay->AppendText(str);
	str.Printf(wxT("CUC:                       %.10g\n"),e->cuc);
	tDisplay->AppendText(str);
	str.Printf(wxT("CUS:                       %.10g\n"),e->cus);
	tDisplay->AppendText(str);
	str.Printf(wxT("CRC:                       %.10g\n"),e->crc);
	tDisplay->AppendText(str);
	str.Printf(wxT("CRS:                       %.10g\n"),e->crs);
	tDisplay->AppendText(str);


}