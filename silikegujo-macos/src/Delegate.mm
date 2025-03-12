/* Delegate.mm: Delegate for Cocoa
 * Copyright 2012-2021 Vincent Damewood
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

#include <Silikego/InfixParser.h>
#include <Silikego/SyntaxTree.h>
#include <Silikego/FunctionCaller.h>
#include <Silikego/StringSource.h>

#import "Delegate.h"

@implementation SilikegujoDelegate


- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
	Silikego::FunctionCaller::SetUp();
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication
{
	return YES;
}

- (void)applicationWillTerminate:(NSNotification *)aNotification
{
	Silikego::FunctionCaller::TearDown();
}

- (IBAction) Calculate:(id)sender
{
	std::unique_ptr<Silikego::SyntaxTreeNode> Ast = Silikego::ParseInfix(
		std::unique_ptr<Silikego::DataSource>(new Silikego::StringSource(
			[[self.input stringValue] UTF8String])));
	Silikego::Value Result = Ast->Evaluate();

	std::string ResultString = Result.ToString();
	NSString *ResultNSString = [[NSString alloc] initWithUTF8String: ResultString.c_str()];
	[self.output setStringValue: ResultNSString];
}
@end
