/***********************************************************************
 *
 * Copyright (C) 2009, 2011 Graeme Gott <graeme@gottcode.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

#ifndef FRAME_H
#define FRAME_H

#include <QWidget>

class Frame : public QWidget {
public:
	Frame(QWidget* parent = 0);

protected:
	virtual void paintEvent(QPaintEvent* event);

	void setHighlight(bool highlight) {
		m_highlight = highlight;
	}

	void setHighlightBorder(bool highlight) {
		m_highlight_border = highlight;
	}

	void setHighlightPartial(bool highlight) {
		m_highlight_partial = highlight;
	}

private:
	bool m_highlight;
	bool m_highlight_border;
	bool m_highlight_partial;
};

#endif
