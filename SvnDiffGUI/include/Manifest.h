/****************************************************************************
 Copyright (c) 2013 cocos2d-x.org
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __Manifest__
#define __Manifest__


#include <string>
#include <unordered_map>
#include <vector>

#include "json/document.h"


class  Manifest 
{
public:
    
    friend class ManifestManager;
    
    //! The type of difference
    enum class DiffType {
        ADDED,
        DELETED,
        MODIFIED
    };
    
    enum class DownloadState {
        UNSTARTED,
        DOWNLOADING,
        SUCCESSED
    };

	
    
    //! Asset object
    struct Asset {
        std::string md5;
        std::string path;
        bool compressed;
        DownloadState downloadState;
    };
    
    //! Object indicate the difference between two Assets
    struct AssetDiff {
        Asset asset;
        DiffType type;
    };


	struct GroupVersionValue{
		std::string vername;  //版本名字
		size_t versize;		//版本大小
	};
    
    /** @brief Check whether the version informations have been fully loaded
     */
    bool isVersionLoaded() const;
    
    /** @brief Check whether the manifest have been fully loaded
     */
    bool isLoaded() const;
    
    /** @brief Gets remote package url.
     */
    const std::string& getPackageUrl() const;

    /** @brief Gets remote manifest file url.
     */
    const std::string& getManifestFileUrl() const;

	void setManifestFileUrl(std::string manifestFileUrl);
	void setVersionFileUrl(std::string versionFileUrl);

	void setPackageUrl(std::string packageUrl);
    
    /** @brief Gets remote version file url.
     */
    const std::string& getVersionFileUrl() const;
    
    /** @brief Gets manifest version.
     */
    const std::string& getVersion() const;
    
    /** @brief Get the search paths list related to the Manifest.
     */
    std::vector<std::string> getSearchPaths() const;
    
protected:
    
    /** @brief Constructor for Manifest class
     * @param manifestUrl Url of the local manifest
     */
    Manifest(const std::string& manifestUrl = "");
    
    /** @brief Load the json file into local json object
     * @param url Url of the json file
     */
	void loadJson(const std::string& content);
    
    /** @brief Parse the version file information into this manifest
     * @param versionUrl Url of the local version file
     */
	void parseVersion(const std::string& content);


	GroupVersionValue parseGroupVersion(const rapidjson::Value &json);
    
    /** @brief Parse the manifest file information into this manifest
     * @param manifestUrl Url of the local manifest
     */
	void parse(const std::string& content);
    
    /** @brief Check whether the version of this manifest equals to another.
     * @param b   The other manifest
     */
    bool versionEquals(const Manifest *b) const;
    
    /** @brief Generate difference between this Manifest and another.
     * @param b   The other manifest
     */
    std::unordered_map<std::string, AssetDiff> genDiff(const Manifest *b) const;

    
    void loadVersion(const rapidjson::Document &json);
    
    void loadManifest(const rapidjson::Document &json);
    
    void saveToFile(const std::string &filepath);
    
    Asset parseAsset(const std::string &path, const rapidjson::Value &json);
    
    void clear();
    
    /** @brief Gets all groups.
    
    const std::vector<std::string>& getGroups() const; */
    
    /** @brief Gets all groups version.
     */
	const std::unordered_map<std::string, GroupVersionValue>& getGroupVerions() const;
    
    /** @brief Gets version for the given group.
     * @param group   Key of the requested group
     */
	const GroupVersionValue& getGroupVersion(const std::string &group) const;
    
    /** @brief Gets assets.
     */
    const std::unordered_map<std::string, Asset>& getAssets() const;

	bool addAsset(std::string name, std::string path, std::string md5, bool isCompress);

	bool addGroupVersion(std::string name, std::string vername, size_t versize);

	bool removeAssets();

	void setVersion(std::string  version);
	void setFixVersion(std::string fixversion);
private:
    
    //! Indicate whether the version informations have been fully loaded
    bool _versionLoaded;
    
    //! Indicate whether the manifest have been fully loaded
    bool _loaded;
    
    
    //! The local manifest root
    std::string _manifestRoot;
    
    //! The remote package url
    std::string _packageUrl;
    
    //! The remote path of manifest file
    std::string _remoteManifestUrl;
    
    //! The remote path of version file [Optional]
    std::string _remoteVersionUrl;
    
    //! The version of local manifest
    std::string _version;

	std::string _fixversion;
    
    //! All groups exist in manifest [Optional]
    std::vector<std::string> _groups;
    
    //! The versions of all local group [Optional]
    std::unordered_map<std::string, Manifest::GroupVersionValue> _groupVer;
    
    //! The version of local engine
    std::string _engineVer;
    
    //! Full assets list
    std::unordered_map<std::string, Asset> _assets;
    
    //! All search paths
    std::vector<std::string> _searchPaths;
    
    rapidjson::Document _json;
};

#endif /* defined(__Manifest__) */
