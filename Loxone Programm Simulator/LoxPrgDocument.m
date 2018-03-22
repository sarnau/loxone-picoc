//
//  LoxPrgDocument.m
//  Loxone Programm Simulator
//
//  Created by Markus Fritze on 16.03.18.
//

#import "LoxPrgDocument.h"
#import "LoxSim.h"

@interface LoxPrgDocument ()
@property NSString *sourceCode;
@end

@implementation LoxPrgDocument {
    Picoc _picoC;
    NSThread *_picoThread;
}

- (instancetype)init {
    self = [super init];
    if (self) {
        self->_picoC.owner = (__bridge void *)(self);
    }
    return self;
}

+ (BOOL)autosavesInPlace {
    return YES;
}

- (NSString *)windowNibName {
    return @"LoxPrgDocument";
}

- (void)awakeFromNib
{
    self.sourceCodeView.automaticQuoteSubstitutionEnabled = false;
    if(self.sourceCode) {
        [self.sourceCodeView.textStorage appendAttributedString:[[NSAttributedString alloc] initWithString:self.sourceCode attributes:@{ NSFontAttributeName:[NSFont userFixedPitchFontOfSize:NSFont.systemFontSize] }]];
        [self.sourceCodeView scrollRangeToVisible:NSMakeRange(0, 0)];
    }

    NSArray<NSTextField *> *textInputs = @[ self.textInput1, self.textInput2, self.textInput3 ];
    for(NSTextField *field in textInputs) {
        field.objectValue = @"";
    } 
    NSArray<NSTextField *> *analogInputs = @[ self.analogInput1, self.analogInput2, self.analogInput3, self.analogInput4, self.analogInput5, self.analogInput6, self.analogInput7, self.analogInput8, self.analogInput9, self.analogInput10, self.analogInput11, self.analogInput12, self.analogInput13 ];
    for(NSTextField *field in analogInputs) {
        field.objectValue = @0;
    }

    [self clearLog:self];
}

- (IBAction)startStopPicoC:(id)sender
{
    if(!_picoThread) {
        _picoThread = LoxSim_Launch(&self->_picoC, self.displayName, self.sourceCodeView.textStorage.string);
        self.runStopToolbarItem.image = [NSImage imageNamed:@"NSGoBackTemplate"];
    } else {
        LoxSim_Stop(&self->_picoC, _picoThread);
        _picoThread = nil;
        self.runStopToolbarItem.image = [NSImage imageNamed:@"NSGoForwardTemplate"];
    }
}

- (IBAction)clearLog:(id)sender
{
    NSArray<NSTextField *> *textOutputs = @[ self.textOutput1, self.textOutput2, self.textOutput3 ];
    for(NSTextField *field in textOutputs) {
        field.objectValue = nil;
    } 
    NSArray<NSTextField *> *analogOutputs = @[ self.analogOutput1, self.analogOutput2, self.analogOutput3, self.analogOutput4, self.analogOutput5, self.analogOutput6, self.analogOutput7, self.analogOutput8, self.analogOutput9, self.analogOutput10, self.analogOutput11, self.analogOutput12, self.analogOutput13 ];
    for(NSTextField *field in analogOutputs) {
        field.objectValue = nil;
    }
    self.errorTextOutput.objectValue = nil; 

    [self.logOutputView.textStorage setAttributedString:[[NSAttributedString alloc] initWithString:@""]];
}

- (IBAction)inputTextUpdated:(id)sender 
{
    LoxSim_triggerInputEvent(&self->_picoC, 1<<[sender tag]);
}

- (IBAction)inputNumberUpdated:(id)sender
{
    LoxSim_triggerInputEvent(&self->_picoC, 1<<([sender tag] + 3));  // 3 text inputs
}

- (NSData *)dataOfType:(NSString *)typeName error:(NSError **)outError
{
    if(self.sourceCodeView.textStorage.string)
        return [self.sourceCodeView.textStorage.string dataUsingEncoding:NSUTF8StringEncoding];
    return nil;
}

- (BOOL)readFromData:(NSData *)data ofType:(NSString *)typeName error:(NSError **)outError
{
    self.sourceCode = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    return YES;
}

@end
