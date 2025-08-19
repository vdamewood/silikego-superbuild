/* Delegate.mm: Delegate for Cocoa
 * Copyright 2012-2025 Vincent Damewood
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

#include <memory>
#import <Cocoa/Cocoa.h>

#include <SilikegoCore/InfixParser.h>
#include <SilikegoCore/SyntaxTree.h>
#include <SilikegoCore/FunctionCaller.h>
#include <SilikegoCore/StringSource.h>

#import "Delegate.h"

@implementation SilikegoGuiDelegate


- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
	self.caller = new Silikego::FunctionCaller();
	self.caller->InstallOperators();
	self.caller->InstallFunctions();
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication
{
	return YES;
}

- (void)applicationWillTerminate:(NSNotification *)aNotification
{
	delete self.caller;
}

- (IBAction) Calculate:(id)sender
{
	std::unique_ptr<Silikego::SyntaxTreeNode> Ast = Silikego::ParseInfix(
		std::unique_ptr<Silikego::DataSource>(new Silikego::StringSource(
			[[self.input stringValue] UTF8String])));
	Silikego::Value Result = Ast->Evaluate(*self.caller);

	switch (Result.Status())
	{
	case Silikego::ValueStatus::INTEGER:
		[self.output setIntegerValue: Result.Integer()];
		break;
	case  Silikego::ValueStatus::FLOAT:
		[self.output setDoubleValue: Result.Float()];
		break;
	case Silikego::ValueStatus::MEMORY_ERR:
		[self.output setStringValue: @"Out of memory"];
		break;
	case Silikego::ValueStatus::SYNTAX_ERR:
		[self.output setStringValue: @"Syntax error"];
		break;
	case Silikego::ValueStatus::ZERO_DIV_ERR:
		[self.output setStringValue: @"Division by zero"];
		break;
	case Silikego::ValueStatus::BAD_FUNCTION:
		[self.output setStringValue: @"Function not found"];
		break;
	case Silikego::ValueStatus::BAD_ARGUMENTS:
		[self.output setStringValue: @"Bad argument count"];
		break;
	case Silikego::ValueStatus::DOMAIN_ERR:
		[self.output setStringValue: @"Domain error"];
		break;
	case Silikego::ValueStatus::RANGE_ERR:
		[self.output setStringValue: @"Range error"];
		break;
	default:
		[self.output setStringValue: @"Unexpected error"];
	}
}
@end
