class Repository < ActiveRecord::Base
  has_many :translation_files, :dependent => :destroy

  def create_translations
    self.translation_files.destroy_all

    %x[find #{self.path} -name "*.po" -printf "%P\n"].split("\n").each do |filepath|
      self.translation_files.find_or_create_by_filepath(filepath).update_translations
    end

    self.update_attribute(:last_commit, %x[cd #{self.path}; git rev-parse HEAD].strip)
  end

  def update_translations
    %x[cd #{self.path}; git diff --name-status --no-renames #{self.last_commit}].
    split("\n").map { |line| line.split("\t") }.each do |mode, filepath|
      case mode
        when 'D'
          self.translation_files.where('filepath = ?', filepath).destroy
        when 'A', 'M'
          self.translation_files.find_or_create_by_filepath(filepath).update_translations
        else
          raise Exception
      end
    end
  end
end
