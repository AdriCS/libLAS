/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  LAS indexing class 
 * Author:   Howard Butler, hobu.inc@gmail.com
 *
 ******************************************************************************
 * Copyright (c) 2009, Howard Butler
 *
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following 
 * conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright 
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright 
 *       notice, this list of conditions and the following disclaimer in 
 *       the documentation and/or other materials provided 
 *       with the distribution.
 *     * Neither the name of the Martin Isenburg or Iowa Department 
 *       of Natural Resources nor the names of its contributors may be 
 *       used to endorse or promote products derived from this software 
 *       without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 ****************************************************************************/

#ifndef LIBLAS_LASINDEX_HPP_INCLUDED
#define LIBLAS_LASINDEX_HPP_INCLUDED

#include <liblas/lasvariablerecord.hpp>
#include <liblas/laspoint.hpp>

#include <spatialindex/SpatialIndex.h>


//std
#include <string>
#include <vector>
#include <stack>
#include <sys/stat.h>

namespace liblas {

class LASDataStream;

class LASIndex
{
public:

    enum IndexType
    {
        eMemoryIndex = 1, ///< A transient memory index that will go away
        eVLRIndex = 2, ///< An index that will store its data in VLR records
        eExternalIndex = 3 ///< An index that will store its data in files alongside the .las file (.las.dat & .las.idx)
    };
    
    LASIndex();
    LASIndex(LASDataStream& strm, std::string& filename);
    LASIndex(std::string& filename);
    /// Copy constructor.
    LASIndex(LASIndex const& other);
    ~LASIndex();
    
    /// Assignment operator.
    LASIndex& operator=(LASIndex const& rhs);
    
    /// Comparison operator.
    bool operator==(const LASIndex& other) const;

    
    void insert(LASPoint& p, int64_t id);
    std::vector<uint32_t>* intersects(double minx, double miny, double maxx, double maxy, double minz, double maxz);

    /// Sets the page size for the index when stored externally
    /// \param v - page value.  Defaults to 4096.
    void SetPageSize(uint32_t v) { m_Pagesize = v; }

    /// Get index page size for indexes that are stored externally
    /// \return index page size.
    uint32_t GetPageSize() { return m_Pagesize; }
    
    /// Sets the index type
    /// \param v - index type. 
    void SetIndexType(IndexType v) { m_idxType = v; }

    /// Gets the index type
    /// \return index type.
    IndexType GetIndexType() { return m_idxType; }
    
private:

    SpatialIndex::IStorageManager* m_storage;
    SpatialIndex::StorageManager::IBuffer* m_buffer;
    SpatialIndex::ISpatialIndex* m_rtree;

    uint32_t m_Pagesize;
    IndexType m_idxType;
    SpatialIndex::id_type m_idxId;
    uint32_t m_idxCapacity;
    uint32_t m_idxLeafCap;
    uint32_t m_idxDimension;   
    double m_idxFillFactor; 
    bool m_idxExternalExists;


    uint16_t m_bufferCapacity;
    bool m_bufferWriteThrough;
    
    void Init();
    SpatialIndex::IStorageManager* CreateStorage(std::string& filename);
    SpatialIndex::StorageManager::IBuffer* CreateIndexBuffer(SpatialIndex::IStorageManager& storage);
    
    bool ExternalIndexExists(std::string& filename);
};

class LASVisitor : public SpatialIndex::IVisitor
{
private:
    size_t m_indexIO;
    size_t m_leafIO;
    std::vector<uint32_t>* m_vector;
    

public:

    LASVisitor(std::vector<uint32_t>* vect);

    void visitNode(const SpatialIndex::INode& n);
    void visitData(const SpatialIndex::IData& d);
    void visitData(std::vector<const SpatialIndex::IData*>& v);
};

class LASDataStream : public SpatialIndex::IDataStream
{
public:
    LASDataStream(LASReader* reader);

    ~LASDataStream()
    {
        if (m_pNext != 0) delete m_pNext;
    };

    SpatialIndex::IData* getNext();
    bool hasNext() throw (Tools::NotSupportedException);

    size_t size() throw (Tools::NotSupportedException);
    void rewind() throw (Tools::NotSupportedException);

    bool readPoint();

protected:
    liblas::LASReader* m_reader;
    SpatialIndex::RTree::Data* m_pNext;
    SpatialIndex::id_type m_id;
};




extern SpatialIndex::IStorageManager* returnLASStorageManager(Tools::PropertySet& in);
extern SpatialIndex::IStorageManager* createNewLASStorageManager();

class LASStorageManager : public SpatialIndex::IStorageManager
{
public:
    LASStorageManager(Tools::PropertySet&);

    virtual ~LASStorageManager();

    virtual void loadByteArray(const SpatialIndex::id_type id, size_t& len, uint8_t** data);
    virtual void storeByteArray(SpatialIndex::id_type& id, const size_t len, const uint8_t* const data);
    virtual void deleteByteArray(const SpatialIndex::id_type id);

private:
    class Entry
    {
    public:
        byte* m_pData;
        size_t m_length;

        Entry(size_t l, const uint8_t* const d) : m_pData(0), m_length(l)
        {
            m_pData = new uint8_t[m_length];
            memcpy(m_pData, d, m_length);
        }

        ~Entry() { delete[] m_pData; }
    }; // Entry

    std::vector<Entry*> m_buffer;
    std::stack<SpatialIndex::id_type> m_emptyPages;
}; // MemoryStorageManager


}

#endif // LIBLAS_LASINDEX_HPP_INCLUDED
