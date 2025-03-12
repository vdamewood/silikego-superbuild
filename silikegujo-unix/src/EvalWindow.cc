/* EvalWindow.cc: Expression evaluation window
 * Copyright 2012-2022 Vincent Damewood
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
 */

#include <string>

#include <gtkmm.h>

#include <Silikego/StringSource.h>
#include <Silikego/InfixParser.h>
#include <Silikego/Value.h>

#include "EvalWindow.h"

static const char UiResource[] = "/com/vdamewood/SilikegujoForUnix/EvalWindow.ui";

EvalWindow* EvalWindow::Create()
{
	auto builder = Gtk::Builder::create_from_resource(UiResource);
	return Gtk::Builder::get_widget_derived<EvalWindow>(builder, "EvalWindow");
}

EvalWindow::EvalWindow(
		BaseObjectType* cobject,
		const Glib::RefPtr<Gtk::Builder>& builder)
	: Gtk::Window(cobject),
	MyBuilder(builder),
	MyButton(builder->get_widget<Gtk::Button>("CalculateButton")),
	MyInput(builder->get_widget<Gtk::Entry>("Input")),
	MyOutput(builder->get_widget<Gtk::Label>("Output"))
{
	MyButton->signal_clicked().connect(
		sigc::mem_fun(
			*this,
			&EvalWindow::Calculate));
	set_default_widget(*MyButton);
	MyInput->set_activates_default(true);
}

void EvalWindow::Calculate()
{
	std::unique_ptr<Silikego::SyntaxTreeNode> ResultTree =
		Silikego::ParseInfix(
		std::unique_ptr<Silikego::DataSource>(new Silikego::StringSource(
			MyInput->get_text().c_str())));
	Silikego::Value Value = ResultTree->Evaluate();

	std::string ResultString = Value.ToString();
	MyOutput->set_text(ResultString.c_str());
}
