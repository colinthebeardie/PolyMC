#pragma once

#include <memory>
#include <QList>
#include <QJsonDocument>
#include <QDateTime>
#include "meta/JsonFormat.h"
#include "ProblemProvider.h"
#include "QObjectPtr.h"
#include "multimc_logic_export.h"

class ComponentList;
class LaunchProfile;
namespace Meta
{
	class Version;
	class VersionList;
}
class VersionFile;

class MULTIMC_LOGIC_EXPORT Component : public QObject, public ProblemProvider
{
Q_OBJECT
public:
	Component(ComponentList * parent, const QString &uid);

	// DEPRECATED: remove these constructors?
	Component(ComponentList * parent, std::shared_ptr<Meta::Version> version);
	Component(ComponentList * parent, const QString & uid, std::shared_ptr<VersionFile> file);

	virtual ~Component(){};
	void applyTo(LaunchProfile *profile);

	bool isMoveable();
	bool isCustomizable();
	bool isRevertible();
	bool isRemovable();
	bool isCustom();
	bool isVersionChangeable();

	// DEPRECATED: explicit numeric order values, used for loading old non-component config. TODO: refactor and move to migration code
	void setOrder(int order);
	int getOrder();

	QString getID();
	QString getName();
	QString getVersion();
	std::shared_ptr<Meta::Version> getMeta();
	QDateTime getReleaseDateTime();

	QString getFilename();

	std::shared_ptr<class VersionFile> getVersionFile() const;
	std::shared_ptr<class Meta::VersionList> getVersionList() const;

	void setImportant (bool state);

	const QList<PatchProblem> getProblems() const override;
	ProblemSeverity getProblemSeverity() const override;

	void setVersion(const QString & version);
	bool customize();
	bool revert();

	void updateCachedData();

signals:
	void dataChanged();

public: /* data */
	ComponentList * m_parent;

	// BEGIN: persistent component list properties
	/// ID of the component
	QString m_uid;
	/// version of the component - when there's a custom json override, this is also the version the component reverts to
	QString m_version;
	/// if true, this has been added automatically to satisfy dependencies and may be automatically removed
	bool m_dependencyOnly = false;
	/// if true, the component is either the main component of the instance, or otherwise important and cannot be removed.
	bool m_important = false;

	/// cached name for display purposes, taken from the version file (meta or local override)
	QString m_cachedName;
	/// cached version for display AND other purposes, taken from the version file (meta or local override)
	QString m_cachedVersion;
	/// cached set of requirements, taken from the version file (meta or local override)
	Meta::RequireSet m_cachedRequires;
	Meta::RequireSet m_cachedConflicts;
	/// if true, the component is volatile and may be automatically removed when no longer needed
	bool m_cachedVolatile = false;
	// END: persistent component list properties

	// DEPRECATED: explicit numeric order values, used for loading old non-component config. TODO: refactor and move to migration code
	bool m_orderOverride = false;
	int m_order = 0;

	// load state
	std::shared_ptr<Meta::Version> m_metaVersion;
	std::shared_ptr<VersionFile> m_file;
	bool m_loaded = false;
};

typedef shared_qobject_ptr<Component> ComponentPtr;
