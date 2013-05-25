#import <Foundation/Foundation.h>
#import "YapAbstractDatabaseExtension.h"

/**
 * Welcome to YapDatabase!
 *
 * The project page has a wealth of documentation if you have any questions.
 * https://github.com/yaptv/YapDatabase
 *
 * If you're new to the project you may want to visit the wiki.
 * https://github.com/yaptv/YapDatabase/wiki
 *
 * This is the base database class which is shared by YapDatabase and YapCollectionsDatabase.
 *
 * - YapDatabase = Key/Value
 * - YapCollectionsDatabase = Collection/Key/Value
 * 
 * You do not directly create instances of YapAbstractDatabase.
 * You instead create instances of YapDatabase or YapCollectionsDatabase.
 * Both YapDatabase and YapCollectionsDatabase extend YapAbstractDatabase.
 * 
 * YapAbstractDatabase provides the generic implementation of a database such as:
 * - common properties
 * - common initializers
 * - common setup code
 * - stub methods which are overriden by the subclasses
 * 
 * @see YapDatabase.h
 * @see YapCollectionsDatabase.h
**/
@interface YapAbstractDatabase : NSObject

#pragma mark Shared class methods

/**
 * How does YapDatabase store my objects to disk?
 *
 * That question is answered extensively in the wiki article "Storing Objects":
 * https://github.com/yaptv/YapDatabase/wiki/Storing-Objects
 * 
 * Here's the intro from the wiki article:
 * 
 * > In order to store an object to disk (via YapDatabase or any other protocol) you need some way of
 * > serializing the object. That is, convert the object into a big blob of bytes. And then, to get your
 * > object back from the disk you deserialize it (convert big blob of bytes back into object form).
 * >
 * > With YapDatabase, you can choose the default serialization/deserialization process,
 * > or you can customize it and use your own routines.
**/

/**
 * The default serializer & deserializer use NSCoding (NSKeyedArchiver & NSKeyedUnarchiver).
 * Thus any objects that support the NSCoding protocol may be used.
 *
 * Many of Apple's primary data types support NSCoding out of the box.
 * It's easy to add NSCoding support to your own custom objects.
**/
+ (NSData *(^)(id object))defaultSerializer;
+ (id (^)(NSData *))defaultDeserializer;

/**
 * Property lists ONLY support the following: NSData, NSString, NSArray, NSDictionary, NSDate, and NSNumber.
 * Property lists are highly optimized and are used extensively Apple.
 * 
 * Property lists make a good fit when your existing code already uses them,
 * such as replacing NSUserDefaults with a database.
**/
+ (NSData *(^)(id object))propertyListSerializer;
+ (id (^)(NSData *))propertyListDeserializer;

/**
 * A FASTER serializer & deserializer than the default, if serializing ONLY a NSDate object.
 * You may want to use timestampSerializer & timestampDeserializer if your metadata is simply an NSDate.
**/
+ (NSData *(^)(id object))timestampSerializer;
+ (id (^)(NSData *))timestampDeserializer;

#pragma mark Init

/**
 * Opens or creates a sqlite database with the given path.
 * The default serializer and deserializer are used.
 * 
 * @see defaultSerializer
 * @see defaultDeserializer
**/
- (id)initWithPath:(NSString *)path;

/**
 * Opens or creates a sqlite database with the given path.
 * The given serializer and deserializer are used for both objects and metadata.
**/
- (id)initWithPath:(NSString *)path
        serializer:(NSData *(^)(id object))serializer
      deserializer:(id (^)(NSData *))deserializer;

/**
 * Opens or creates a sqlite database with the given path.
 * The given serializers and deserializers are used.
**/
- (id)initWithPath:(NSString *)path objectSerializer:(NSData *(^)(id object))objectSerializer
                                  objectDeserializer:(id (^)(NSData *))objectDeserializer
                                  metadataSerializer:(NSData *(^)(id object))metadataSerializer
                                metadataDeserializer:(id (^)(NSData *))metadataDeserializer;

#pragma mark Properties

@property (nonatomic, strong, readonly) NSString *databasePath;

@property (nonatomic, strong, readonly) NSData *(^objectSerializer)(id object);
@property (nonatomic, strong, readonly) id (^objectDeserializer)(NSData *data);

@property (nonatomic, strong, readonly) NSData *(^metadataSerializer)(id object);
@property (nonatomic, strong, readonly) id (^metadataDeserializer)(NSData *data);


#pragma mark Extensions

/**
 * Registers the extension with the database using the given name.
 * After registration everything works automatically using just the extension name.
 * 
 * @return
 *     YES if the extension was properly registered.
 *     NO if an error occurred, such as the extensionName is already registered.
**/
- (BOOL)registerExtension:(YapAbstractDatabaseExtension *)extension withName:(NSString *)extensionName;

/**
 * Returns the registered extension with the given name.
 * The returned object will be a subclass of YapAbstractDatabaseExtension.
**/
- (id)registeredExtension:(NSString *)extensionName;

/**
 * Returns all currently registered extensions as a dictionary.
 * The key is the registed name (NSString), and the value is the extension (YapAbstractDatabaseExtension subclass).
**/
- (NSDictionary *)registeredExtensions;

@end