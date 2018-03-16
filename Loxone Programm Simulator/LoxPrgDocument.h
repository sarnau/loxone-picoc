//
//  LoxPrgDocument.h
//  Loxone Programm Simulator
//
//  Created by Markus Fritze on 16.03.18.
//

#import <Cocoa/Cocoa.h>

@interface LoxPrgDocument : NSDocument
@property (weak) IBOutlet NSTextField *programmName;
@property (weak) IBOutlet NSTextField *textInput1;
@property (weak) IBOutlet NSTextField *textInput2;
@property (weak) IBOutlet NSTextField *textInput3;
@property (weak) IBOutlet NSTextField *analogInput1;
@property (weak) IBOutlet NSTextField *analogInput2;
@property (weak) IBOutlet NSTextField *analogInput3;
@property (weak) IBOutlet NSTextField *analogInput4;
@property (weak) IBOutlet NSTextField *analogInput5;
@property (weak) IBOutlet NSTextField *analogInput6;
@property (weak) IBOutlet NSTextField *analogInput7;
@property (weak) IBOutlet NSTextField *analogInput8;
@property (weak) IBOutlet NSTextField *analogInput9;
@property (weak) IBOutlet NSTextField *analogInput10;
@property (weak) IBOutlet NSTextField *analogInput11;
@property (weak) IBOutlet NSTextField *analogInput12;
@property (weak) IBOutlet NSTextField *analogInput13;
@property (weak) IBOutlet NSTextField *textOutput1;
@property (weak) IBOutlet NSTextField *textOutput2;
@property (weak) IBOutlet NSTextField *textOutput3;
@property (weak) IBOutlet NSTextField *analogOutput1;
@property (weak) IBOutlet NSTextField *analogOutput2;
@property (weak) IBOutlet NSTextField *analogOutput3;
@property (weak) IBOutlet NSTextField *analogOutput4;
@property (weak) IBOutlet NSTextField *analogOutput5;
@property (weak) IBOutlet NSTextField *analogOutput6;
@property (weak) IBOutlet NSTextField *analogOutput7;
@property (weak) IBOutlet NSTextField *analogOutput8;
@property (weak) IBOutlet NSTextField *analogOutput9;
@property (weak) IBOutlet NSTextField *analogOutput10;
@property (weak) IBOutlet NSTextField *analogOutput11;
@property (weak) IBOutlet NSTextField *analogOutput12;
@property (weak) IBOutlet NSTextField *analogOutput13;
@property (weak) IBOutlet NSTextField *errorTextOutput;
@property (unsafe_unretained) IBOutlet NSTextView *logOutputView;
@property (unsafe_unretained) IBOutlet NSTextView *sourceCodeView;
@property (weak) IBOutlet NSToolbarItem *runStopToolbarItem;
@end

