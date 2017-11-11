/* Copyright 2015-2017 MultiMC Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <QJsonObject>
#include <memory>

#include "Exception.h"
#include "meta/BaseEntity.h"
#include <set>

namespace Meta
{
class Index;
class Version;
class VersionList;

class ParseException : public Exception
{
public:
	using Exception::Exception;
};
struct Require
{
	bool operator==(const Require & rhs) const
	{
		return uid == rhs.uid;
	}
	bool operator<(const Require & rhs) const
	{
		return uid < rhs.uid;
	}
	bool deepEquals(const Require & rhs) const
	{
		return uid == rhs.uid
			&& equalsVersion == rhs.equalsVersion
			&& suggests == rhs.suggests;
	}
	QString uid;
	QString equalsVersion;
	QString suggests;
};

inline Q_DECL_PURE_FUNCTION uint qHash(const Require &key, uint seed = 0) Q_DECL_NOTHROW
{
	return qHash(key.uid, seed);
}

using RequireSet = std::set<Require>;

void parseIndex(const QJsonObject &obj, Index *ptr);
void parseVersion(const QJsonObject &obj, Version *ptr);
void parseVersionList(const QJsonObject &obj, VersionList *ptr);

// FIXME: this has a different shape than the others...FIX IT!?
void parseRequires(const QJsonObject &obj, RequireSet * ptr, const char * keyName = "requires");
void serializeRequires(QJsonObject & objOut, RequireSet* ptr, const char * keyName = "requires");
}

Q_DECLARE_METATYPE(std::set<Meta::Require>);