/***********************************************************************
 *
 * Copyright (C) 2008, 2009, 2013 Graeme Gott <graeme@gottcode.org>
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

#include "dancing_links.h"

namespace DLX
{

/*****************************************************************************/

Matrix::Matrix(unsigned int max_columns)
: m_max_columns(max_columns), m_columns(max_columns), m_output(max_columns), m_solutions(0), m_tries(0) {
	m_header = new HeaderNode;
	m_header->column = m_header;

	Node* node = m_header;
	HeaderNode* column = 0;
	for (unsigned int i = 0; i < m_max_columns; ++i) {
		column = &m_columns[i];
		column->id = i;
		column->up = column->down = column->column = column;
		column->left = node;
		node->right = column;
		node = column;
	}
	node->right = m_header;
}

/*****************************************************************************/

Matrix::~Matrix() {
	delete m_header;
}

/*****************************************************************************/

void Matrix::addRow() {
	m_rows.append(HeaderNode());
	HeaderNode* row = &m_rows.last();
	row->left = row->right = row->up = row->down = row->column = row;
}

/*****************************************************************************/

void Matrix::addElement(unsigned int c) {
	Q_ASSERT(c < m_max_columns);

	HeaderNode* column = &m_columns[c];
	HeaderNode* row = &m_rows.last();

	m_nodes.append(Node());
	Node* node = &m_nodes.last();

	node->left = row->left;
	node->right = row;
	row->left->right = node;
	row->left = node;

	node->up = column->up;
	node->down = column;
	column->up->down = node;
	column->up = node;

	node->column = column;

	column->size++;
}

/*****************************************************************************/

unsigned int Matrix::search(Callback* solution, unsigned int max_solutions, unsigned int max_tries) {
	m_solution = solution;

	m_solutions = 0;
	m_max_solutions = max_solutions;

	m_tries = 0;
	m_max_tries = (max_tries != 0) ? max_tries : m_max_columns;

	solve(0);
	return m_solutions;
}

/*****************************************************************************/

void Matrix::solve(unsigned int k) {
	if (m_header->right == m_header) {
		++m_solutions;
		(*m_solution)(m_output, k);
		return;
	}

	if ((m_solutions >= m_max_solutions) || (++m_tries >= m_max_tries)) {
		return;
	}

	HeaderNode* column = 0;
	unsigned int s = 0xFFFFFFFF;
	for(HeaderNode* i = m_header->right->column; i != m_header; i = i->right->column) {
		if (i->size < s) {
			column = i;
			s = i->size;
		}
	}

	cover(column);

	unsigned int next_k = k + 1;

	for(Node* row = column->down; row != column; row = row->down) {
		m_output[k] = row;

		for(Node* j = row->right; j != row; j = j->right) {
			cover(j->column);
		}

		solve(next_k);

		row = m_output[k];
		column = row->column;

		for(Node* j = row->left; j != row; j = j->left) {
			uncover(j->column);
		}
	}

	uncover(column);
}

/*****************************************************************************/

void Matrix::cover(HeaderNode* node) {
	node->right->left = node->left;
	node->left->right = node->right;

	for (Node* i = node->down; i != node; i = i->down) {
		for (Node* j = i->right; j != i; j = j->right) {
			j->down->up = j->up;
			j->up->down = j->down;
			j->column->size--;
		}
	}
}

/*****************************************************************************/

void Matrix::uncover(HeaderNode* node) {
	for (Node* i = node->up; i != node; i = i->up) {
		for (Node* j = i->left; j != i; j = j->left) {
			j->column->size++;
			j->down->up = j;
			j->up->down = j;
		}
	}

	node->right->left = node;
	node->left->right = node;
}

/*****************************************************************************/

}
